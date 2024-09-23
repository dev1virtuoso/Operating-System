// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Universal power supply monitor class
 *
 *  Copyright © 2007  Anton Vorontsov <cbou@mail.ru>
 *  Copyright © 2004  Szabolcs Gyurko
 *  Copyright © 2003  Ian Molton <spyro@f2s.com>
 *
 *  Modified: 2004, Oct     Szabolcs Gyurko
 */

#include <linux/cleanup.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/notifier.h>
#include <linux/err.h>
#include <linux/of.h>
#include <linux/power_supply.h>
#include <linux/property.h>
#include <linux/thermal.h>
#include <linux/fixp-arith.h>
#include "power_supply.h"
#include "samsung-sdi-battery.h"

static const struct class power_supply_class = {
	.name = "power_supply",
	.dev_uevent = power_supply_uevent,
};

static BLOCKING_NOTIFIER_HEAD(power_supply_notifier);

static const struct device_type power_supply_dev_type = {
	.name = "power_supply",
	.groups = power_supply_attr_groups,
};

#define POWER_SUPPLY_DEFERRED_REGISTER_TIME	msecs_to_jiffies(10)

static bool __power_supply_is_supplied_by(struct power_supply *supplier,
					 struct power_supply *supply)
{
	int i;

	if (!supply->supplied_from && !supplier->supplied_to)
		return false;

	/* Support both supplied_to and supplied_from modes */
	if (supply->supplied_from) {
		if (!supplier->desc->name)
			return false;
		for (i = 0; i < supply->num_supplies; i++)
			if (!strcmp(supplier->desc->name, supply->supplied_from[i]))
				return true;
	} else {
		if (!supply->desc->name)
			return false;
		for (i = 0; i < supplier->num_supplicants; i++)
			if (!strcmp(supplier->supplied_to[i], supply->desc->name))
				return true;
	}

	return false;
}

static int __power_supply_changed_work(struct device *dev, void *data)
{
	struct power_supply *psy = data;
	struct power_supply *pst = dev_get_drvdata(dev);

	if (__power_supply_is_supplied_by(psy, pst)) {
		if (pst->desc->external_power_changed)
			pst->desc->external_power_changed(pst);
	}

	return 0;
}

static void power_supply_changed_work(struct work_struct *work)
{
	unsigned long flags;
	struct power_supply *psy = container_of(work, struct power_supply,
						changed_work);

	dev_dbg(&psy->dev, "%s\n", __func__);

	spin_lock_irqsave(&psy->changed_lock, flags);
	/*
	 * Check 'changed' here to avoid issues due to race between
	 * power_supply_changed() and this routine. In worst case
	 * power_supply_changed() can be called again just before we take above
	 * lock. During the first call of this routine we will mark 'changed' as
	 * false and it will stay false for the next call as well.
	 */
	if (likely(psy->changed)) {
		psy->changed = false;
		spin_unlock_irqrestore(&psy->changed_lock, flags);
		power_supply_for_each_device(psy, __power_supply_changed_work);
		power_supply_update_leds(psy);
		blocking_notifier_call_chain(&power_supply_notifier,
				PSY_EVENT_PROP_CHANGED, psy);
		kobject_uevent(&psy->dev.kobj, KOBJ_CHANGE);
		spin_lock_irqsave(&psy->changed_lock, flags);
	}

	/*
	 * Hold the wakeup_source until all events are processed.
	 * power_supply_changed() might have called again and have set 'changed'
	 * to true.
	 */
	if (likely(!psy->changed))
		pm_relax(&psy->dev);
	spin_unlock_irqrestore(&psy->changed_lock, flags);
}

int power_supply_for_each_device(void *data, int (*fn)(struct device *dev, void *data))
{
	return class_for_each_device(&power_supply_class, NULL, data, fn);
}
EXPORT_SYMBOL_GPL(power_supply_for_each_device);

void power_supply_changed(struct power_supply *psy)
{
	unsigned long flags;

	dev_dbg(&psy->dev, "%s\n", __func__);

	spin_lock_irqsave(&psy->changed_lock, flags);
	psy->changed = true;
	pm_stay_awake(&psy->dev);
	spin_unlock_irqrestore(&psy->changed_lock, flags);
	schedule_work(&psy->changed_work);
}
EXPORT_SYMBOL_GPL(power_supply_changed);

/*
 * Notify that power supply was registered after parent finished the probing.
 *
 * Often power supply is registered from driver's probe function. However
 * calling power_supply_changed() directly from power_supply_register()
 * would lead to execution of get_property() function provided by the driver
 * too early - before the probe ends.
 *
 * Avoid that by waiting on parent's mutex.
 */
static void power_supply_deferred_register_work(struct work_struct *work)
{
	struct power_supply *psy = container_of(work, struct power_supply,
						deferred_register_work.work);

	if (psy->dev.parent) {
		while (!mutex_trylock(&psy->dev.parent->mutex)) {
			if (psy->removing)
				return;
			msleep(10);
		}
	}

	power_supply_changed(psy);

	if (psy->dev.parent)
		mutex_unlock(&psy->dev.parent->mutex);
}

#ifdef CONFIG_OF
static int __power_supply_populate_supplied_from(struct device *dev,
						 void *data)
{
	struct power_supply *psy = data;
	struct power_supply *epsy = dev_get_drvdata(dev);
	struct device_node *np;
	int i = 0;

	do {
		np = of_parse_phandle(psy->of_node, "power-supplies", i++);
		if (!np)
			break;

		if (np == epsy->of_node) {
			dev_dbg(&psy->dev, "%s: Found supply : %s\n",
				psy->desc->name, epsy->desc->name);
			psy->supplied_from[i-1] = (char *)epsy->desc->name;
			psy->num_supplies++;
			of_node_put(np);
			break;
		}
		of_node_put(np);
	} while (np);

	return 0;
}

static int power_supply_populate_supplied_from(struct power_supply *psy)
{
	int error;

	error = power_supply_for_each_device(psy, __power_supply_populate_supplied_from);

	dev_dbg(&psy->dev, "%s %d\n", __func__, error);

	return error;
}

static int  __power_supply_find_supply_from_node(struct device *dev,
						 void *data)
{
	struct device_node *np = data;
	struct power_supply *epsy = dev_get_drvdata(dev);

	/* returning non-zero breaks out of power_supply_for_each_device loop */
	if (epsy->of_node == np)
		return 1;

	return 0;
}

static int power_supply_find_supply_from_node(struct device_node *supply_node)
{
	int error;

	/*
	 * power_supply_for_each_device() either returns its own errors or values
	 * returned by __power_supply_find_supply_from_node().
	 *
	 * __power_supply_find_supply_from_node() will return 0 (no match)
	 * or 1 (match).
	 *
	 * We return 0 if power_supply_for_each_device() returned 1, -EPROBE_DEFER if
	 * it returned 0, or error as returned by it.
	 */
	error = power_supply_for_each_device(supply_node, __power_supply_find_supply_from_node);

	return error ? (error == 1 ? 0 : error) : -EPROBE_DEFER;
}

static int power_supply_check_supplies(struct power_supply *psy)
{
	struct device_node *np;
	int cnt = 0;

	/* If there is already a list honor it */
	if (psy->supplied_from && psy->num_supplies > 0)
		return 0;

	/* No device node found, nothing to do */
	if (!psy->of_node)
		return 0;

	do {
		int ret;

		np = of_parse_phandle(psy->of_node, "power-supplies", cnt++);
		if (!np)
			break;

		ret = power_supply_find_supply_from_node(np);
		of_node_put(np);

		if (ret) {
			dev_dbg(&psy->dev, "Failed to find supply!\n");
			return ret;
		}
	} while (np);

	/* Missing valid "power-supplies" entries */
	if (cnt == 1)
		return 0;

	/* All supplies found, allocate char ** array for filling */
	psy->supplied_from = devm_kzalloc(&psy->dev, sizeof(*psy->supplied_from),
					  GFP_KERNEL);
	if (!psy->supplied_from)
		return -ENOMEM;

	*psy->supplied_from = devm_kcalloc(&psy->dev,
					   cnt - 1, sizeof(**psy->supplied_from),
					   GFP_KERNEL);
	if (!*psy->supplied_from)
		return -ENOMEM;

	return power_supply_populate_supplied_from(psy);
}
#else
static int power_supply_check_supplies(struct power_supply *psy)
{
	int nval, ret;

	if (!psy->dev.parent)
		return 0;

	nval = device_property_string_array_count(psy->dev.parent, "supplied-from");
	if (nval <= 0)
		return 0;

	psy->supplied_from = devm_kmalloc_array(&psy->dev, nval,
						sizeof(char *), GFP_KERNEL);
	if (!psy->supplied_from)
		return -ENOMEM;

	ret = device_property_read_string_array(psy->dev.parent,
		"supplied-from", (const char **)psy->supplied_from, nval);
	if (ret < 0)
		return ret;

	psy->num_supplies = nval;

	return 0;
}
#endif

struct psy_am_i_supplied_data {
	struct power_supply *psy;
	unsigned int count;
};

static int __power_supply_am_i_supplied(struct device *dev, void *_data)
{
	union power_supply_propval ret = {0,};
	struct power_supply *epsy = dev_get_drvdata(dev);
	struct psy_am_i_supplied_data *data = _data;

	if (__power_supply_is_supplied_by(epsy, data->psy)) {
		data->count++;
		if (!epsy->desc->get_property(epsy, POWER_SUPPLY_PROP_ONLINE,
					&ret))
			return ret.intval;
	}

	return 0;
}

int power_supply_am_i_supplied(struct power_supply *psy)
{
	struct psy_am_i_supplied_data data = { psy, 0 };
	int error;

	error = power_supply_for_each_device(&data, __power_supply_am_i_supplied);

	dev_dbg(&psy->dev, "%s count %u err %d\n", __func__, data.count, error);

	if (data.count == 0)
		return -ENODEV;

	return error;
}
EXPORT_SYMBOL_GPL(power_supply_am_i_supplied);

static int __power_supply_is_system_supplied(struct device *dev, void *data)
{
	union power_supply_propval ret = {0,};
	struct power_supply *psy = dev_get_drvdata(dev);
	unsigned int *count = data;

	if (!psy->desc->get_property(psy, POWER_SUPPLY_PROP_SCOPE, &ret))
		if (ret.intval == POWER_SUPPLY_SCOPE_DEVICE)
			return 0;

	(*count)++;
	if (psy->desc->type != POWER_SUPPLY_TYPE_BATTERY)
		if (!psy->desc->get_property(psy, POWER_SUPPLY_PROP_ONLINE,
					&ret))
			return ret.intval;

	return 0;
}

int power_supply_is_system_supplied(void)
{
	int error;
	unsigned int count = 0;

	error = power_supply_for_each_device(&count, __power_supply_is_system_supplied);

	/*
	 * If no system scope power class device was found at all, most probably we
	 * are running on a desktop system, so assume we are on mains power.
	 */
	if (count == 0)
		return 1;

	return error;
}
EXPORT_SYMBOL_GPL(power_supply_is_system_supplied);

struct psy_get_supplier_prop_data {
	struct power_supply *psy;
	enum power_supply_property psp;
	union power_supply_propval *val;
};

static int __power_supply_get_supplier_property(struct device *dev, void *_data)
{
	struct power_supply *epsy = dev_get_drvdata(dev);
	struct psy_get_supplier_prop_data *data = _data;

	if (__power_supply_is_supplied_by(epsy, data->psy))
		if (!power_supply_get_property(epsy, data->psp, data->val))
			return 1; /* Success */

	return 0; /* Continue iterating */
}

int power_supply_get_property_from_supplier(struct power_supply *psy,
					    enum power_supply_property psp,
					    union power_supply_propval *val)
{
	struct psy_get_supplier_prop_data data = {
		.psy = psy,
		.psp = psp,
		.val = val,
	};
	int ret;

	/*
	 * This function is not intended for use with a supply with multiple
	 * suppliers, we simply pick the first supply to report the psp.
	 */
	ret = power_supply_for_each_device(&data, __power_supply_get_supplier_property);
	if (ret < 0)
		return ret;
	if (ret == 0)
		return -ENODEV;

	return 0;
}
EXPORT_SYMBOL_GPL(power_supply_get_property_from_supplier);

int power_supply_set_battery_charged(struct power_supply *psy)
{
	if (atomic_read(&psy->use_cnt) >= 0 &&
			psy->desc->type == POWER_SUPPLY_TYPE_BATTERY &&
			psy->desc->set_charged) {
		psy->desc->set_charged(psy);
		return 0;
	}

	return -EINVAL;
}
EXPORT_SYMBOL_GPL(power_supply_set_battery_charged);

static int power_supply_match_device_by_name(struct device *dev, const void *data)
{
	const char *name = data;
	struct power_supply *psy = dev_get_drvdata(dev);

	return strcmp(psy->desc->name, name) == 0;
}

/**
 * power_supply_get_by_name() - Search for a power supply and returns its ref
 * @name: Power supply name to fetch
 *
 * If power supply was found, it increases reference count for the
 * internal power supply's device. The user should power_supply_put()
 * after usage.
 *
 * Return: On success returns a reference to a power supply with
 * matching name equals to @name, a NULL otherwise.
 */
struct power_supply *power_supply_get_by_name(const char *name)
{
	struct power_supply *psy = NULL;
	struct device *dev = class_find_device(&power_supply_class, NULL, name,
					       power_supply_match_device_by_name);

	if (dev) {
		psy = dev_get_drvdata(dev);
		atomic_inc(&psy->use_cnt);
	}

	return psy;
}
EXPORT_SYMBOL_GPL(power_supply_get_by_name);

/**
 * power_supply_put() - Drop reference obtained with power_supply_get_by_name
 * @psy: Reference to put
 *
 * The reference to power supply should be put before unregistering
 * the power supply.
 */
void power_supply_put(struct power_supply *psy)
{
	might_sleep();

	atomic_dec(&psy->use_cnt);
	put_device(&psy->dev);
}
EXPORT_SYMBOL_GPL(power_supply_put);

#ifdef CONFIG_OF
static int power_supply_match_device_node(struct device *dev, const void *data)
{
	return dev->parent && dev->parent->of_node == data;
}

/**
 * power_supply_get_by_phandle() - Search for a power supply and returns its ref
 * @np: Pointer to device node holding phandle property
 * @property: Name of property holding a power supply name
 *
 * If power supply was found, it increases reference count for the
 * internal power supply's device. The user should power_supply_put()
 * after usage.
 *
 * Return: On success returns a reference to a power supply with
 * matching name equals to value under @property, NULL or ERR_PTR otherwise.
 */
struct power_supply *power_supply_get_by_phandle(struct device_node *np,
							const char *property)
{
	struct device_node *power_supply_np;
	struct power_supply *psy = NULL;
	struct device *dev;

	power_supply_np = of_parse_phandle(np, property, 0);
	if (!power_supply_np)
		return ERR_PTR(-ENODEV);

	dev = class_find_device(&power_supply_class, NULL, power_supply_np,
				power_supply_match_device_node);

	of_node_put(power_supply_np);

	if (dev) {
		psy = dev_get_drvdata(dev);
		atomic_inc(&psy->use_cnt);
	}

	return psy;
}
EXPORT_SYMBOL_GPL(power_supply_get_by_phandle);

static void devm_power_supply_put(struct device *dev, void *res)
{
	struct power_supply **psy = res;

	power_supply_put(*psy);
}

/**
 * devm_power_supply_get_by_phandle() - Resource managed version of
 *  power_supply_get_by_phandle()
 * @dev: Pointer to device holding phandle property
 * @property: Name of property holding a power supply phandle
 *
 * Return: On success returns a reference to a power supply with
 * matching name equals to value under @property, NULL or ERR_PTR otherwise.
 */
struct power_supply *devm_power_supply_get_by_phandle(struct device *dev,
						      const char *property)
{
	struct power_supply **ptr, *psy;

	if (!dev->of_node)
		return ERR_PTR(-ENODEV);

	ptr = devres_alloc(devm_power_supply_put, sizeof(*ptr), GFP_KERNEL);
	if (!ptr)
		return ERR_PTR(-ENOMEM);

	psy = power_supply_get_by_phandle(dev->of_node, property);
	if (IS_ERR_OR_NULL(psy)) {
		devres_free(ptr);
	} else {
		*ptr = psy;
		devres_add(dev, ptr);
	}
	return psy;
}
EXPORT_SYMBOL_GPL(devm_power_supply_get_by_phandle);
#endif /* CONFIG_OF */

int power_supply_get_battery_info(struct power_supply *psy,
				  struct power_supply_battery_info **info_out)
{
	struct power_supply_resistance_temp_table *resist_table;
	struct power_supply_battery_info *info;
	struct device_node *battery_np = NULL;
	struct fwnode_reference_args args;
	struct fwnode_handle *fwnode = NULL;
	const char *value;
	int err, len, index;
	const __be32 *list;
	u32 min_max[2];

	if (psy->of_node) {
		battery_np = of_parse_phandle(psy->of_node, "monitored-battery", 0);
		if (!battery_np)
			return -ENODEV;

		fwnode = fwnode_handle_get(of_fwnode_handle(battery_np));
	} else if (psy->dev.parent) {
		err = fwnode_property_get_reference_args(
					dev_fwnode(psy->dev.parent),
					"monitored-battery", NULL, 0, 0, &args);
		if (err)
			return err;

		fwnode = args.fwnode;
	}

	if (!fwnode)
		return -ENOENT;

	err = fwnode_property_read_string(fwnode, "compatible", &value);
	if (err)
		goto out_put_node;


	/* Try static batteries first */
	err = samsung_sdi_battery_get_info(&psy->dev, value, &info);
	if (!err)
		goto out_ret_pointer;
	else if (err == -ENODEV)
		/*
		 * Device does not have a static battery.
		 * Proceed to look for a simple battery.
		 */
		err = 0;

	if (strcmp("simple-battery", value)) {
		err = -ENODEV;
		goto out_put_node;
	}

	info = devm_kzalloc(&psy->dev, sizeof(*info), GFP_KERNEL);
	if (!info) {
		err = -ENOMEM;
		goto out_put_node;
	}

	info->technology                     = POWER_SUPPLY_TECHNOLOGY_UNKNOWN;
	info->energy_full_design_uwh         = -EINVAL;
	info->charge_full_design_uah         = -EINVAL;
	info->voltage_min_design_uv          = -EINVAL;
	info->voltage_max_design_uv          = -EINVAL;
	info->precharge_current_ua           = -EINVAL;
	info->charge_term_current_ua         = -EINVAL;
	info->constant_charge_current_max_ua = -EINVAL;
	info->constant_charge_voltage_max_uv = -EINVAL;
	info->tricklecharge_current_ua       = -EINVAL;
	info->precharge_voltage_max_uv       = -EINVAL;
	info->charge_restart_voltage_uv      = -EINVAL;
	info->overvoltage_limit_uv           = -EINVAL;
	info->maintenance_charge             = NULL;
	info->alert_low_temp_charge_current_ua = -EINVAL;
	info->alert_low_temp_charge_voltage_uv = -EINVAL;
	info->alert_high_temp_charge_current_ua = -EINVAL;
	info->alert_high_temp_charge_voltage_uv = -EINVAL;
	info->temp_ambient_alert_min         = INT_MIN;
	info->temp_ambient_alert_max         = INT_MAX;
	info->temp_alert_min                 = INT_MIN;
	info->temp_alert_max                 = INT_MAX;
	info->temp_min                       = INT_MIN;
	info->temp_max                       = INT_MAX;
	info->factory_internal_resistance_uohm  = -EINVAL;
	info->resist_table                   = NULL;
	info->bti_resistance_ohm             = -EINVAL;
	info->bti_resistance_tolerance       = -EINVAL;

	for (index = 0; index < POWER_SUPPLY_OCV_TEMP_MAX; index++) {
		info->ocv_table[index]       = NULL;
		info->ocv_temp[index]        = -EINVAL;
		info->ocv_table_size[index]  = -EINVAL;
	}

	/* The property and field names below must correspond to elements
	 * in enum power_supply_property. For reasoning, see
	 * Documentation/power/power_supply_class.rst.
	 */

	if (!fwnode_property_read_string(fwnode, "device-chemistry", &value)) {
		if (!strcmp("nickel-cadmium", value))
			info->technology = POWER_SUPPLY_TECHNOLOGY_NiCd;
		else if (!strcmp("nickel-metal-hydride", value))
			info->technology = POWER_SUPPLY_TECHNOLOGY_NiMH;
		else if (!strcmp("lithium-ion", value))
			/* Imprecise lithium-ion type */
			info->technology = POWER_SUPPLY_TECHNOLOGY_LION;
		else if (!strcmp("lithium-ion-polymer", value))
			info->technology = POWER_SUPPLY_TECHNOLOGY_LIPO;
		else if (!strcmp("lithium-ion-iron-phosphate", value))
			info->technology = POWER_SUPPLY_TECHNOLOGY_LiFe;
		else if (!strcmp("lithium-ion-manganese-oxide", value))
			info->technology = POWER_SUPPLY_TECHNOLOGY_LiMn;
		else
			dev_warn(&psy->dev, "%s unknown battery type\n", value);
	}

	fwnode_property_read_u32(fwnode, "energy-full-design-microwatt-hours",
			     &info->energy_full_design_uwh);
	fwnode_property_read_u32(fwnode, "charge-full-design-microamp-hours",
			     &info->charge_full_design_uah);
	fwnode_property_read_u32(fwnode, "voltage-min-design-microvolt",
			     &info->voltage_min_design_uv);
	fwnode_property_read_u32(fwnode, "voltage-max-design-microvolt",
			     &info->voltage_max_design_uv);
	fwnode_property_read_u32(fwnode, "trickle-charge-current-microamp",
			     &info->tricklecharge_current_ua);
	fwnode_property_read_u32(fwnode, "precharge-current-microamp",
			     &info->precharge_current_ua);
	fwnode_property_read_u32(fwnode, "precharge-upper-limit-microvolt",
			     &info->precharge_voltage_max_uv);
	fwnode_property_read_u32(fwnode, "charge-term-current-microamp",
			     &info->charge_term_current_ua);
	fwnode_property_read_u32(fwnode, "re-charge-voltage-microvolt",
			     &info->charge_restart_voltage_uv);
	fwnode_property_read_u32(fwnode, "over-voltage-threshold-microvolt",
			     &info->overvoltage_limit_uv);
	fwnode_property_read_u32(fwnode, "constant-charge-current-max-microamp",
			     &info->constant_charge_current_max_ua);
	fwnode_property_read_u32(fwnode, "constant-charge-voltage-max-microvolt",
			     &info->constant_charge_voltage_max_uv);
	fwnode_property_read_u32(fwnode, "factory-internal-resistance-micro-ohms",
			     &info->factory_internal_resistance_uohm);

	if (!fwnode_property_read_u32_array(fwnode, "ambient-celsius",
					    min_max, ARRAY_SIZE(min_max))) {
		info->temp_ambient_alert_min = min_max[0];
		info->temp_ambient_alert_max = min_max[1];
	}
	if (!fwnode_property_read_u32_array(fwnode, "alert-celsius",
					    min_max, ARRAY_SIZE(min_max))) {
		info->temp_alert_min = min_max[0];
		info->temp_alert_max = min_max[1];
	}
	if (!fwnode_property_read_u32_array(fwnode, "operating-range-celsius",
					    min_max, ARRAY_SIZE(min_max))) {
		info->temp_min = min_max[0];
		info->temp_max = min_max[1];
	}

	/*
	 * The below code uses raw of-data parsing to parse
	 * /schemas/types.yaml#/definitions/uint32-matrix
	 * data, so for now this is only support with of.
	 */
	if (!battery_np)
		goto out_ret_pointer;

	len = of_property_count_u32_elems(battery_np, "ocv-capacity-celsius");
	if (len < 0 && len != -EINVAL) {
		err = len;
		goto out_put_node;
	} else if (len > POWER_SUPPLY_OCV_TEMP_MAX) {
		dev_err(&psy->dev, "Too many temperature values\n");
		err = -EINVAL;
		goto out_put_node;
	} else if (len > 0) {
		of_property_read_u32_array(battery_np, "ocv-capacity-celsius",
					   info->ocv_temp, len);
	}

	for (index = 0; index < len; index++) {
		struct power_supply_battery_ocv_table *table;
		int i, tab_len, size;

		char *propname __free(kfree) = kasprintf(GFP_KERNEL, "ocv-capacity-table-%d",
							 index);
		if (!propname) {
			power_supply_put_battery_info(psy, info);
			err = -ENOMEM;
			goto out_put_node;
		}
		list = of_get_property(battery_np, propname, &size);
		if (!list || !size) {
			dev_err(&psy->dev, "failed to get %s\n", propname);
			power_supply_put_battery_info(psy, info);
			err = -EINVAL;
			goto out_put_node;
		}

		tab_len = size / (2 * sizeof(__be32));
		info->ocv_table_size[index] = tab_len;

		table = info->ocv_table[index] =
			devm_kcalloc(&psy->dev, tab_len, sizeof(*table), GFP_KERNEL);
		if (!info->ocv_table[index]) {
			power_supply_put_battery_info(psy, info);
			err = -ENOMEM;
			goto out_put_node;
		}

		for (i = 0; i < tab_len; i++) {
			table[i].ocv = be32_to_cpu(*list);
			list++;
			table[i].capacity = be32_to_cpu(*list);
			list++;
		}
	}

	list = of_get_property(battery_np, "resistance-temp-table", &len);
	if (!list || !len)
		goto out_ret_pointer;

	info->resist_table_size = len / (2 * sizeof(__be32));
	resist_table = info->resist_table = devm_kcalloc(&psy->dev,
							 info->resist_table_size,
							 sizeof(*resist_table),
							 GFP_KERNEL);
	if (!info->resist_table) {
		power_supply_put_battery_info(psy, info);
		err = -ENOMEM;
		goto out_put_node;
	}

	for (index = 0; index < info->resist_table_size; index++) {
		resist_table[index].temp = be32_to_cpu(*list++);
		resist_table[index].resistance = be32_to_cpu(*list++);
	}

out_ret_pointer:
	/* Finally return the whole thing */
	*info_out = info;

out_put_node:
	fwnode_handle_put(fwnode);
	of_node_put(battery_np);
	return err;
}
EXPORT_SYMBOL_GPL(power_supply_get_battery_info);

void power_supply_put_battery_info(struct power_supply *psy,
				   struct power_supply_battery_info *info)
{
	int i;

	for (i = 0; i < POWER_SUPPLY_OCV_TEMP_MAX; i++) {
		if (info->ocv_table[i])
			devm_kfree(&psy->dev, info->ocv_table[i]);
	}

	if (info->resist_table)
		devm_kfree(&psy->dev, info->resist_table);

	devm_kfree(&psy->dev, info);
}
EXPORT_SYMBOL_GPL(power_supply_put_battery_info);

const enum power_supply_property power_supply_battery_info_properties[] = {
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN,
	POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN,
	POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN,
	POWER_SUPPLY_PROP_PRECHARGE_CURRENT,
	POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX,
	POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX,
	POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MIN,
	POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MAX,
	POWER_SUPPLY_PROP_TEMP_ALERT_MIN,
	POWER_SUPPLY_PROP_TEMP_ALERT_MAX,
	POWER_SUPPLY_PROP_TEMP_MIN,
	POWER_SUPPLY_PROP_TEMP_MAX,
};
EXPORT_SYMBOL_GPL(power_supply_battery_info_properties);

const size_t power_supply_battery_info_properties_size = ARRAY_SIZE(power_supply_battery_info_properties);
EXPORT_SYMBOL_GPL(power_supply_battery_info_properties_size);

bool power_supply_battery_info_has_prop(struct power_supply_battery_info *info,
					enum power_supply_property psp)
{
	if (!info)
		return false;

	switch (psp) {
	case POWER_SUPPLY_PROP_TECHNOLOGY:
		return info->technology != POWER_SUPPLY_TECHNOLOGY_UNKNOWN;
	case POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN:
		return info->energy_full_design_uwh >= 0;
	case POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN:
		return info->charge_full_design_uah >= 0;
	case POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN:
		return info->voltage_min_design_uv >= 0;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
		return info->voltage_max_design_uv >= 0;
	case POWER_SUPPLY_PROP_PRECHARGE_CURRENT:
		return info->precharge_current_ua >= 0;
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		return info->charge_term_current_ua >= 0;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
		return info->constant_charge_current_max_ua >= 0;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX:
		return info->constant_charge_voltage_max_uv >= 0;
	case POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MIN:
		return info->temp_ambient_alert_min > INT_MIN;
	case POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MAX:
		return info->temp_ambient_alert_max < INT_MAX;
	case POWER_SUPPLY_PROP_TEMP_ALERT_MIN:
		return info->temp_alert_min > INT_MIN;
	case POWER_SUPPLY_PROP_TEMP_ALERT_MAX:
		return info->temp_alert_max < INT_MAX;
	case POWER_SUPPLY_PROP_TEMP_MIN:
		return info->temp_min > INT_MIN;
	case POWER_SUPPLY_PROP_TEMP_MAX:
		return info->temp_max < INT_MAX;
	default:
		return false;
	}
}
EXPORT_SYMBOL_GPL(power_supply_battery_info_has_prop);

int power_supply_battery_info_get_prop(struct power_supply_battery_info *info,
				       enum power_supply_property psp,
				       union power_supply_propval *val)
{
	if (!info)
		return -EINVAL;

	if (!power_supply_battery_info_has_prop(info, psp))
		return -EINVAL;

	switch (psp) {
	case POWER_SUPPLY_PROP_TECHNOLOGY:
		val->intval = info->technology;
		return 0;
	case POWER_SUPPLY_PROP_ENERGY_FULL_DESIGN:
		val->intval = info->energy_full_design_uwh;
		return 0;
	case POWER_SUPPLY_PROP_CHARGE_FULL_DESIGN:
		val->intval = info->charge_full_design_uah;
		return 0;
	case POWER_SUPPLY_PROP_VOLTAGE_MIN_DESIGN:
		val->intval = info->voltage_min_design_uv;
		return 0;
	case POWER_SUPPLY_PROP_VOLTAGE_MAX_DESIGN:
		val->intval = info->voltage_max_design_uv;
		return 0;
	case POWER_SUPPLY_PROP_PRECHARGE_CURRENT:
		val->intval = info->precharge_current_ua;
		return 0;
	case POWER_SUPPLY_PROP_CHARGE_TERM_CURRENT:
		val->intval = info->charge_term_current_ua;
		return 0;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_CURRENT_MAX:
		val->intval = info->constant_charge_current_max_ua;
		return 0;
	case POWER_SUPPLY_PROP_CONSTANT_CHARGE_VOLTAGE_MAX:
		val->intval = info->constant_charge_voltage_max_uv;
		return 0;
	case POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MIN:
		val->intval = info->temp_ambient_alert_min;
		return 0;
	case POWER_SUPPLY_PROP_TEMP_AMBIENT_ALERT_MAX:
		val->intval = info->temp_ambient_alert_max;
		return 0;
	case POWER_SUPPLY_PROP_TEMP_ALERT_MIN:
		val->intval = info->temp_alert_min;
		return 0;
	case POWER_SUPPLY_PROP_TEMP_ALERT_MAX:
		val->intval = info->temp_alert_max;
		return 0;
	case POWER_SUPPLY_PROP_TEMP_MIN:
		val->intval = info->temp_min;
		return 0;
	case POWER_SUPPLY_PROP_TEMP_MAX:
		val->intval = info->temp_max;
		return 0;
	default:
		return -EINVAL;
	}
}
EXPORT_SYMBOL_GPL(power_supply_battery_info_get_prop);

/**
 * power_supply_temp2resist_simple() - find the battery internal resistance
 * percent from temperature
 * @table: Pointer to battery resistance temperature table
 * @table_len: The table length
 * @temp: Current temperature
 *
 * This helper function is used to look up battery internal resistance percent
 * according to current temperature value from the resistance temperature table,
 * and the table must be ordered descending. Then the actual battery internal
 * resistance = the ideal battery internal resistance * percent / 100.
 *
 * Return: the battery internal resistance percent
 */
int power_supply_temp2resist_simple(struct power_supply_resistance_temp_table *table,
				    int table_len, int temp)
{
	int i, high, low;

	for (i = 0; i < table_len; i++)
		if (temp > table[i].temp)
			break;

	/* The library function will deal with high == low */
	if (i == 0)
		high = low = i;
	else if (i == table_len)
		high = low = i - 1;
	else
		high = (low = i) - 1;

	return fixp_linear_interpolate(table[low].temp,
				       table[low].resistance,
				       table[high].temp,
				       table[high].resistance,
				       temp);
}
EXPORT_SYMBOL_GPL(power_supply_temp2resist_simple);

/**
 * power_supply_vbat2ri() - find the battery internal resistance
 * from the battery voltage
 * @info: The battery information container
 * @vbat_uv: The battery voltage in microvolt
 * @charging: If we are charging (true) or not (false)
 *
 * This helper function is used to look up battery internal resistance
 * according to current battery voltage. Depending on whether the battery
 * is currently charging or not, different resistance will be returned.
 *
 * Returns the internal resistance in microohm or negative error code.
 */
int power_supply_vbat2ri(struct power_supply_battery_info *info,
			 int vbat_uv, bool charging)
{
	const struct power_supply_vbat_ri_table *vbat2ri;
	int table_len;
	int i, high, low;

	/*
	 * If we are charging, and the battery supplies a separate table
	 * for this state, we use that in order to compensate for the
	 * charging voltage. Otherwise we use the main table.
	 */
	if (charging && info->vbat2ri_charging) {
		vbat2ri = info->vbat2ri_charging;
		table_len = info->vbat2ri_charging_size;
	} else {
		vbat2ri = info->vbat2ri_discharging;
		table_len = info->vbat2ri_discharging_size;
	}

	/*
	 * If no tables are specified, or if we are above the highest voltage in
	 * the voltage table, just return the factory specified internal resistance.
	 */
	if (!vbat2ri || (table_len <= 0) || (vbat_uv > vbat2ri[0].vbat_uv)) {
		if (charging && (info->factory_internal_resistance_charging_uohm > 0))
			return info->factory_internal_resistance_charging_uohm;
		else
			return info->factory_internal_resistance_uohm;
	}

	/* Break loop at table_len - 1 because that is the highest index */
	for (i = 0; i < table_len - 1; i++)
		if (vbat_uv > vbat2ri[i].vbat_uv)
			break;

	/* The library function will deal with high == low */
	if ((i == 0) || (i == (table_len - 1)))
		high = i;
	else
		high = i - 1;
	low = i;

	return fixp_linear_interpolate(vbat2ri[low].vbat_uv,
				       vbat2ri[low].ri_uohm,
				       vbat2ri[high].vbat_uv,
				       vbat2ri[high].ri_uohm,
				       vbat_uv);
}
EXPORT_SYMBOL_GPL(power_supply_vbat2ri);

const struct power_supply_maintenance_charge_table *
power_supply_get_maintenance_charging_setting(struct power_supply_battery_info *info,
					      int index)
{
	if (index >= info->maintenance_charge_size)
		return NULL;
	return &info->maintenance_charge[index];
}
EXPORT_SYMBOL_GPL(power_supply_get_maintenance_charging_setting);

/**
 * power_supply_ocv2cap_simple() - find the battery capacity
 * @table: Pointer to battery OCV lookup table
 * @table_len: OCV table length
 * @ocv: Current OCV value
 *
 * This helper function is used to look up battery capacity according to
 * current OCV value from one OCV table, and the OCV table must be ordered
 * descending.
 *
 * Return: the battery capacity.
 */
int power_supply_ocv2cap_simple(struct power_supply_battery_ocv_table *table,
				int table_len, int ocv)
{
	int i, high, low;

	for (i = 0; i < table_len; i++)
		if (ocv > table[i].ocv)
			break;

	/* The library function will deal with high == low */
	if (i == 0)
		high = low = i;
	else if (i == table_len)
		high = low = i - 1;
	else
		high = (low = i) - 1;

	return fixp_linear_interpolate(table[low].ocv,
				       table[low].capacity,
				       table[high].ocv,
				       table[high].capacity,
				       ocv);
}
EXPORT_SYMBOL_GPL(power_supply_ocv2cap_simple);

struct power_supply_battery_ocv_table *
power_supply_find_ocv2cap_table(struct power_supply_battery_info *info,
				int temp, int *table_len)
{
	int best_temp_diff = INT_MAX, temp_diff;
	u8 i, best_index = 0;

	if (!info->ocv_table[0])
		return NULL;

	for (i = 0; i < POWER_SUPPLY_OCV_TEMP_MAX; i++) {
		/* Out of capacity tables */
		if (!info->ocv_table[i])
			break;

		temp_diff = abs(info->ocv_temp[i] - temp);

		if (temp_diff < best_temp_diff) {
			best_temp_diff = temp_diff;
			best_index = i;
		}
	}

	*table_len = info->ocv_table_size[best_index];
	return info->ocv_table[best_index];
}
EXPORT_SYMBOL_GPL(power_supply_find_ocv2cap_table);

int power_supply_batinfo_ocv2cap(struct power_supply_battery_info *info,
				 int ocv, int temp)
{
	struct power_supply_battery_ocv_table *table;
	int table_len;

	table = power_supply_find_ocv2cap_table(info, temp, &table_len);
	if (!table)
		return -EINVAL;

	return power_supply_ocv2cap_simple(table, table_len, ocv);
}
EXPORT_SYMBOL_GPL(power_supply_batinfo_ocv2cap);

bool power_supply_battery_bti_in_range(struct power_supply_battery_info *info,
				       int resistance)
{
	int low, high;

	/* Nothing like this can be checked */
	if (info->bti_resistance_ohm <= 0)
		return false;

	/* This will be extremely strict and unlikely to work */
	if (info->bti_resistance_tolerance <= 0)
		return (info->bti_resistance_ohm == resistance);

	low = info->bti_resistance_ohm -
		(info->bti_resistance_ohm * info->bti_resistance_tolerance) / 100;
	high = info->bti_resistance_ohm +
		(info->bti_resistance_ohm * info->bti_resistance_tolerance) / 100;

	return ((resistance >= low) && (resistance <= high));
}
EXPORT_SYMBOL_GPL(power_supply_battery_bti_in_range);

static bool psy_has_property(const struct power_supply_desc *psy_desc,
			     enum power_supply_property psp)
{
	bool found = false;
	int i;

	for (i = 0; i < psy_desc->num_properties; i++) {
		if (psy_desc->properties[i] == psp) {
			found = true;
			break;
		}
	}

	return found;
}

int power_supply_get_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    union power_supply_propval *val)
{
	if (atomic_read(&psy->use_cnt) <= 0) {
		if (!psy->initialized)
			return -EAGAIN;
		return -ENODEV;
	}

	if (psy_has_property(psy->desc, psp))
		return psy->desc->get_property(psy, psp, val);
	else if (power_supply_battery_info_has_prop(psy->battery_info, psp))
		return power_supply_battery_info_get_prop(psy->battery_info, psp, val);
	else
		return -EINVAL;
}
EXPORT_SYMBOL_GPL(power_supply_get_property);

int power_supply_set_property(struct power_supply *psy,
			    enum power_supply_property psp,
			    const union power_supply_propval *val)
{
	if (atomic_read(&psy->use_cnt) <= 0 || !psy->desc->set_property)
		return -ENODEV;

	return psy->desc->set_property(psy, psp, val);
}
EXPORT_SYMBOL_GPL(power_supply_set_property);

int power_supply_property_is_writeable(struct power_supply *psy,
					enum power_supply_property psp)
{
	return psy->desc->property_is_writeable && psy->desc->property_is_writeable(psy, psp);
}
EXPORT_SYMBOL_GPL(power_supply_property_is_writeable);

void power_supply_external_power_changed(struct power_supply *psy)
{
	if (atomic_read(&psy->use_cnt) <= 0 ||
			!psy->desc->external_power_changed)
		return;

	psy->desc->external_power_changed(psy);
}
EXPORT_SYMBOL_GPL(power_supply_external_power_changed);

int power_supply_powers(struct power_supply *psy, struct device *dev)
{
	return sysfs_create_link(&psy->dev.kobj, &dev->kobj, "powers");
}
EXPORT_SYMBOL_GPL(power_supply_powers);

static void power_supply_dev_release(struct device *dev)
{
	struct power_supply *psy = to_power_supply(dev);

	dev_dbg(dev, "%s\n", __func__);
	kfree(psy);
}

int power_supply_reg_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&power_supply_notifier, nb);
}
EXPORT_SYMBOL_GPL(power_supply_reg_notifier);

void power_supply_unreg_notifier(struct notifier_block *nb)
{
	blocking_notifier_chain_unregister(&power_supply_notifier, nb);
}
EXPORT_SYMBOL_GPL(power_supply_unreg_notifier);

#ifdef CONFIG_THERMAL
static int power_supply_read_temp(struct thermal_zone_device *tzd,
		int *temp)
{
	struct power_supply *psy;
	union power_supply_propval val;
	int ret;

	WARN_ON(tzd == NULL);
	psy = thermal_zone_device_priv(tzd);
	ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_TEMP, &val);
	if (ret)
		return ret;

	/* Convert tenths of degree Celsius to milli degree Celsius. */
	*temp = val.intval * 100;

	return ret;
}

static const struct thermal_zone_device_ops psy_tzd_ops = {
	.get_temp = power_supply_read_temp,
};

static int psy_register_thermal(struct power_supply *psy)
{
	int ret;

	if (psy->desc->no_thermal)
		return 0;

	/* Register battery zone device psy reports temperature */
	if (psy_has_property(psy->desc, POWER_SUPPLY_PROP_TEMP)) {
		/* Prefer our hwmon device and avoid duplicates */
		struct thermal_zone_params tzp = {
			.no_hwmon = IS_ENABLED(CONFIG_POWER_SUPPLY_HWMON)
		};
		psy->tzd = thermal_tripless_zone_device_register(psy->desc->name,
				psy, &psy_tzd_ops, &tzp);
		if (IS_ERR(psy->tzd))
			return PTR_ERR(psy->tzd);
		ret = thermal_zone_device_enable(psy->tzd);
		if (ret)
			thermal_zone_device_unregister(psy->tzd);
		return ret;
	}

	return 0;
}

static void psy_unregister_thermal(struct power_supply *psy)
{
	if (IS_ERR_OR_NULL(psy->tzd))
		return;
	thermal_zone_device_unregister(psy->tzd);
}

#else
static int psy_register_thermal(struct power_supply *psy)
{
	return 0;
}

static void psy_unregister_thermal(struct power_supply *psy)
{
}
#endif

static struct power_supply *__must_check
__power_supply_register(struct device *parent,
				   const struct power_supply_desc *desc,
				   const struct power_supply_config *cfg,
				   bool ws)
{
	struct device *dev;
	struct power_supply *psy;
	int rc;

	if (!desc || !desc->name || !desc->properties || !desc->num_properties)
		return ERR_PTR(-EINVAL);

	if (!parent)
		pr_warn("%s: Expected proper parent device for '%s'\n",
			__func__, desc->name);

	psy = kzalloc(sizeof(*psy), GFP_KERNEL);
	if (!psy)
		return ERR_PTR(-ENOMEM);

	dev = &psy->dev;

	device_initialize(dev);

	dev->class = &power_supply_class;
	dev->type = &power_supply_dev_type;
	dev->parent = parent;
	dev->release = power_supply_dev_release;
	dev_set_drvdata(dev, psy);
	psy->desc = desc;
	if (cfg) {
		dev->groups = cfg->attr_grp;
		psy->drv_data = cfg->drv_data;
		psy->of_node =
			cfg->fwnode ? to_of_node(cfg->fwnode) : cfg->of_node;
		dev->of_node = psy->of_node;
		psy->supplied_to = cfg->supplied_to;
		psy->num_supplicants = cfg->num_supplicants;
	}

	rc = dev_set_name(dev, "%s", desc->name);
	if (rc)
		goto dev_set_name_failed;

	INIT_WORK(&psy->changed_work, power_supply_changed_work);
	INIT_DELAYED_WORK(&psy->deferred_register_work,
			  power_supply_deferred_register_work);

	rc = power_supply_check_supplies(psy);
	if (rc) {
		dev_dbg(dev, "Not all required supplies found, defer probe\n");
		goto check_supplies_failed;
	}

	/*
	 * Expose constant battery info, if it is available. While there are
	 * some chargers accessing constant battery data, we only want to
	 * expose battery data to userspace for battery devices.
	 */
	if (desc->type == POWER_SUPPLY_TYPE_BATTERY) {
		rc = power_supply_get_battery_info(psy, &psy->battery_info);
		if (rc && rc != -ENODEV && rc != -ENOENT)
			goto check_supplies_failed;
	}

	spin_lock_init(&psy->changed_lock);
	rc = device_add(dev);
	if (rc)
		goto device_add_failed;

	rc = device_init_wakeup(dev, ws);
	if (rc)
		goto wakeup_init_failed;

	rc = psy_register_thermal(psy);
	if (rc)
		goto register_thermal_failed;

	rc = power_supply_create_triggers(psy);
	if (rc)
		goto create_triggers_failed;

	rc = power_supply_add_hwmon_sysfs(psy);
	if (rc)
		goto add_hwmon_sysfs_failed;

	/*
	 * Update use_cnt after any uevents (most notably from device_add()).
	 * We are here still during driver's probe but
	 * the power_supply_uevent() calls back driver's get_property
	 * method so:
	 * 1. Driver did not assigned the returned struct power_supply,
	 * 2. Driver could not finish initialization (anything in its probe
	 *    after calling power_supply_register()).
	 */
	atomic_inc(&psy->use_cnt);
	psy->initialized = true;

	queue_delayed_work(system_power_efficient_wq,
			   &psy->deferred_register_work,
			   POWER_SUPPLY_DEFERRED_REGISTER_TIME);

	return psy;

add_hwmon_sysfs_failed:
	power_supply_remove_triggers(psy);
create_triggers_failed:
	psy_unregister_thermal(psy);
register_thermal_failed:
wakeup_init_failed:
	device_del(dev);
device_add_failed:
check_supplies_failed:
dev_set_name_failed:
	put_device(dev);
	return ERR_PTR(rc);
}

/**
 * power_supply_register() - Register new power supply
 * @parent:	Device to be a parent of power supply's device, usually
 *		the device which probe function calls this
 * @desc:	Description of power supply, must be valid through whole
 *		lifetime of this power supply
 * @cfg:	Run-time specific configuration accessed during registering,
 *		may be NULL
 *
 * Return: A pointer to newly allocated power_supply on success
 * or ERR_PTR otherwise.
 * Use power_supply_unregister() on returned power_supply pointer to release
 * resources.
 */
struct power_supply *__must_check power_supply_register(struct device *parent,
		const struct power_supply_desc *desc,
		const struct power_supply_config *cfg)
{
	return __power_supply_register(parent, desc, cfg, true);
}
EXPORT_SYMBOL_GPL(power_supply_register);

/**
 * power_supply_register_no_ws() - Register new non-waking-source power supply
 * @parent:	Device to be a parent of power supply's device, usually
 *		the device which probe function calls this
 * @desc:	Description of power supply, must be valid through whole
 *		lifetime of this power supply
 * @cfg:	Run-time specific configuration accessed during registering,
 *		may be NULL
 *
 * Return: A pointer to newly allocated power_supply on success
 * or ERR_PTR otherwise.
 * Use power_supply_unregister() on returned power_supply pointer to release
 * resources.
 */
struct power_supply *__must_check
power_supply_register_no_ws(struct device *parent,
		const struct power_supply_desc *desc,
		const struct power_supply_config *cfg)
{
	return __power_supply_register(parent, desc, cfg, false);
}
EXPORT_SYMBOL_GPL(power_supply_register_no_ws);

static void devm_power_supply_release(struct device *dev, void *res)
{
	struct power_supply **psy = res;

	power_supply_unregister(*psy);
}

/**
 * devm_power_supply_register() - Register managed power supply
 * @parent:	Device to be a parent of power supply's device, usually
 *		the device which probe function calls this
 * @desc:	Description of power supply, must be valid through whole
 *		lifetime of this power supply
 * @cfg:	Run-time specific configuration accessed during registering,
 *		may be NULL
 *
 * Return: A pointer to newly allocated power_supply on success
 * or ERR_PTR otherwise.
 * The returned power_supply pointer will be automatically unregistered
 * on driver detach.
 */
struct power_supply *__must_check
devm_power_supply_register(struct device *parent,
		const struct power_supply_desc *desc,
		const struct power_supply_config *cfg)
{
	struct power_supply **ptr, *psy;

	ptr = devres_alloc(devm_power_supply_release, sizeof(*ptr), GFP_KERNEL);

	if (!ptr)
		return ERR_PTR(-ENOMEM);
	psy = __power_supply_register(parent, desc, cfg, true);
	if (IS_ERR(psy)) {
		devres_free(ptr);
	} else {
		*ptr = psy;
		devres_add(parent, ptr);
	}
	return psy;
}
EXPORT_SYMBOL_GPL(devm_power_supply_register);

/**
 * devm_power_supply_register_no_ws() - Register managed non-waking-source power supply
 * @parent:	Device to be a parent of power supply's device, usually
 *		the device which probe function calls this
 * @desc:	Description of power supply, must be valid through whole
 *		lifetime of this power supply
 * @cfg:	Run-time specific configuration accessed during registering,
 *		may be NULL
 *
 * Return: A pointer to newly allocated power_supply on success
 * or ERR_PTR otherwise.
 * The returned power_supply pointer will be automatically unregistered
 * on driver detach.
 */
struct power_supply *__must_check
devm_power_supply_register_no_ws(struct device *parent,
		const struct power_supply_desc *desc,
		const struct power_supply_config *cfg)
{
	struct power_supply **ptr, *psy;

	ptr = devres_alloc(devm_power_supply_release, sizeof(*ptr), GFP_KERNEL);

	if (!ptr)
		return ERR_PTR(-ENOMEM);
	psy = __power_supply_register(parent, desc, cfg, false);
	if (IS_ERR(psy)) {
		devres_free(ptr);
	} else {
		*ptr = psy;
		devres_add(parent, ptr);
	}
	return psy;
}
EXPORT_SYMBOL_GPL(devm_power_supply_register_no_ws);

/**
 * power_supply_unregister() - Remove this power supply from system
 * @psy:	Pointer to power supply to unregister
 *
 * Remove this power supply from the system. The resources of power supply
 * will be freed here or on last power_supply_put() call.
 */
void power_supply_unregister(struct power_supply *psy)
{
	WARN_ON(atomic_dec_return(&psy->use_cnt));
	psy->removing = true;
	cancel_work_sync(&psy->changed_work);
	cancel_delayed_work_sync(&psy->deferred_register_work);
	sysfs_remove_link(&psy->dev.kobj, "powers");
	power_supply_remove_hwmon_sysfs(psy);
	power_supply_remove_triggers(psy);
	psy_unregister_thermal(psy);
	device_init_wakeup(&psy->dev, false);
	device_unregister(&psy->dev);
}
EXPORT_SYMBOL_GPL(power_supply_unregister);

void *power_supply_get_drvdata(struct power_supply *psy)
{
	return psy->drv_data;
}
EXPORT_SYMBOL_GPL(power_supply_get_drvdata);

static int __init power_supply_class_init(void)
{
	power_supply_init_attrs();
	return class_register(&power_supply_class);
}

static void __exit power_supply_class_exit(void)
{
	class_unregister(&power_supply_class);
}

subsys_initcall(power_supply_class_init);
module_exit(power_supply_class_exit);

MODULE_DESCRIPTION("Universal power supply monitor class");
MODULE_AUTHOR("Ian Molton <spyro@f2s.com>");
MODULE_AUTHOR("Szabolcs Gyurko");
MODULE_AUTHOR("Anton Vorontsov <cbou@mail.ru>");
