#include <string.h>

#include "scs_config_handlers.hpp"

// Plug-in
#include "ets2-telemetry-common.hpp"
#include "sharedmemory.hpp"

extern SharedMemory *telemMem;
extern ets2TelemetryMap_t *telemPtr;

const scsConfigHandler_t scsConfigTable[] = {
	
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand_id, handleTruckMakeId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_brand, handleTruckMake },	
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_id, handleId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_accessory_id, handleCargoId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_name, handleTruckModel },

	// Truck technical info
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_capacity, handleFuelCapacity },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_fuel_warning_factor, handleFuelWarningFactor },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_adblue_capacity, handleAdblueCapacity },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_warning, handleAirPressureWarning },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_air_pressure_emergency, handleAirPressureEmergency },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_oil_pressure_warning, handleOilPressureWarning },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_water_temperature_warning, handleWaterTemperatureWarning },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_battery_voltage_warning, handleBatteryVoltageWarning },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_rpm_limit, handleRpmLimit },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_forward_gear_count, handleFGearCount },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_reverse_gear_count, handleRGearCount },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_retarder_step_count, handleRetarderStepCount },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cabin_position, handleCabinPosition },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_head_position, handleHeadPosition },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_hook_position, handleHookPosition },
	/*{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_position, handleWheelPosition },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_count, handleWheelCount },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_steerable, handleWheelSteerable },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_simulated, handleWheelSimulated },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_radius, handleWheelRadius },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_powered, handleWheelPowered },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_wheel_liftable, handleWheelLiftable },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_selector_count, handleSelectorCount },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_gear, handleSlotGear },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_handle_position, handleSlotHandlePosition },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_slot_selectors, handleSlotSelectors },*/
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_shifter_type, handleShifterType },

	// Job information
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_income, handleJobIncome },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_delivery_time, handleJobDeadline },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_mass, handleTrailerMass },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo_id, handleTrailerId },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_cargo, handleTrailerName },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_city, handleCitySrc },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_city, handleCityDst },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_source_company, handleCompSrc },
	{ SCS_TELEMETRY_CONFIG_ATTRIBUTE_destination_company, handleCompDst },
};

#define NO_OF_CFGS ( sizeof(scsConfigTable)/sizeof(scsConfigHandler_t) )

bool handleCfg(const scs_named_value_t* current)
{
	int i = 0;

	for (i = 0; i < NO_OF_CFGS; i++)
	{
		if (strcmp(scsConfigTable[i].id, current->name) == 0)
		{
			// Equal ID's; then handle this configuration
			if (scsConfigTable[i].handle)
				scsConfigTable[i].handle(current);

			return true;
		}
	}
	return false;
}

scsConfigHandle(Id)
{
	char * strPtr;

	// ID is shared between vehicle & chassis.
	// So examples could be: vehicle.scania_r and chassis.trailer.overweighl_w
	if (current->value.value_string.value[0] == 'v')
	{
		// Vehicle ID
		// vehicle.scania_r
		strPtr = static_cast<char*>(telemMem->getPtrAt(telemPtr->tel_rev1.modelType[0]));
		strcpy(strPtr, current->value.value_string.value);
		telemPtr->tel_rev1.modelType[1] = strlen(current->value.value_string.value);
				
	}
}

scsConfigHandle(TruckMake)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev3.truckMake, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}
scsConfigHandle(TruckMakeId)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev3.truckMakeId, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}
scsConfigHandle(TruckModel)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev3.truckModel, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}

scsConfigHandle(CargoId)
{
	char * strPtr;

	// Cargo ID
	// Example: cargo.overweighl_w.kvn
	// Cargo type overweighl_w.kvn can be found in def/cargo/
	strPtr = static_cast<char*>(telemMem->getPtrAt(telemPtr->tel_rev1.trailerType[0]));
	strcpy(strPtr, current->value.value_string.value);
	telemPtr->tel_rev1.trailerType[1] = strlen(current->value.value_string.value);\
}

scsConfigHandle(FuelCapacity)
{
	// Fuel capacity
	// Float
	telemPtr->tel_rev1.fuelCapacity = current->value.value_float.value;
}

scsConfigHandle(RpmLimit)
{
	// RPM Limit (often 2500)
	// Float
	telemPtr->tel_rev1.engineRpmMax = current->value.value_float.value;
}

scsConfigHandle(FGearCount)
{
	// No. of drive gears
	// u32
	telemPtr->tel_rev1.gears = current->value.value_u32.value;
}

scsConfigHandle(RGearCount)
{
	// No. of reverse gears
	// u32
	telemPtr->tel_rev2.gears_reverse = current->value.value_u32.value;
}

scsConfigHandle(JobIncome)
{
	if (telemPtr)
	{
		telemPtr->tel_rev2.jobIncome = current->value.value_u64.value;
        job_buffer.jobIncome = current->value.value_u64.value;
	}
}

scsConfigHandle(JobDeadline)
{
	if (telemPtr)
	{
		telemPtr->tel_rev2.time_abs_delivery = current->value.value_u32.value;
        job_buffer.time_abs_delivery = current->value.value_u32.value;
	}
}

scsConfigHandle(TrailerMass)
{
	if (telemPtr)
	{
		telemPtr->tel_rev2.trailerMass = current->value.value_float.value;
        job_buffer.trailerMass = current->value.value_float.value;
	}
}

scsConfigHandle(TrailerId)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.trailerId, current->value.value_string.value, GENERAL_STRING_SIZE);
        strncpy(job_buffer.trailerId, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}

scsConfigHandle(TrailerName)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.trailerName, current->value.value_string.value, GENERAL_STRING_SIZE);
        strncpy(job_buffer.trailerName, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}

scsConfigHandle(CitySrc)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.citySrc, current->value.value_string.value, GENERAL_STRING_SIZE);
        strncpy(job_buffer.citySrc, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}

scsConfigHandle(CityDst)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.cityDst, current->value.value_string.value, GENERAL_STRING_SIZE);
        strncpy(job_buffer.cityDst, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}

scsConfigHandle(CompSrc)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.compSrc, current->value.value_string.value, GENERAL_STRING_SIZE);
        strncpy(job_buffer.compSrc, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}

scsConfigHandle(CompDst)
{
	if (telemPtr)
	{
		strncpy(telemPtr->tel_rev2.compDst, current->value.value_string.value, GENERAL_STRING_SIZE);
        strncpy(job_buffer.compDst, current->value.value_string.value, GENERAL_STRING_SIZE);
	}
}

// rev4

scsConfigHandle(FuelWarningFactor) { telemPtr->tel_rev4.fuelWarningFactor = current->value.value_float.value; }
scsConfigHandle(AdblueCapacity) { telemPtr->tel_rev4.adblueCapacity = current->value.value_float.value; }
scsConfigHandle(AirPressureWarning) { telemPtr->tel_rev4.airPressureWarningValue = current->value.value_float.value; }
scsConfigHandle(AirPressureEmergency) { telemPtr->tel_rev4.airPressureEmergencyValue = current->value.value_float.value; }
scsConfigHandle(OilPressureWarning) { telemPtr->tel_rev4.oilPressureWarningValue = current->value.value_float.value; }
scsConfigHandle(WaterTemperatureWarning) { telemPtr->tel_rev4.waterTemperatureWarningValue = current->value.value_float.value; }
scsConfigHandle(BatteryVoltageWarning) { telemPtr->tel_rev4.batteryVoltageWarningValue = current->value.value_float.value; }

scsConfigHandle(RetarderStepCount) { telemPtr->tel_rev4.retarderStepCount = current->value.value_u32.value; }

scsConfigHandle(CabinPosition) {
	telemPtr->tel_rev4.cabinPositionX = current->value.value_fvector.x;
	telemPtr->tel_rev4.cabinPositionY = current->value.value_fvector.y;
	telemPtr->tel_rev4.cabinPositionZ = current->value.value_fvector.z;
}
scsConfigHandle(HeadPosition) {
	telemPtr->tel_rev4.headPositionX = current->value.value_fvector.x;
	telemPtr->tel_rev4.headPositionY = current->value.value_fvector.y;
	telemPtr->tel_rev4.headPositionZ = current->value.value_fvector.z;
}
scsConfigHandle(HookPosition) {
	telemPtr->tel_rev4.hookPositionX = current->value.value_fvector.x;
	telemPtr->tel_rev4.hookPositionY = current->value.value_fvector.y;
	telemPtr->tel_rev4.hookPositionZ = current->value.value_fvector.z;
}

/*
scsConfigHandle(WheelCount) { telemPtr->tel_rev4.wheelCount = current->value.value_u32.value; }
scsConfigHandle(WheelPosition) {
	if (current->index >= MAX_WHEEL_COUNT) return;
	telemPtr->tel_rev4.wheelPositionX[current->index] = current->value.value_fvector.x;
	telemPtr->tel_rev4.wheelPositionY[current->index] = current->value.value_fvector.y;
	telemPtr->tel_rev4.wheelPositionZ[current->index] = current->value.value_fvector.z;
}
scsConfigHandle(WheelSteerable) {
	if (current->index >= MAX_WHEEL_COUNT) return;
	telemPtr->tel_rev4.wheelSteerable[current->index] = current->value.value_bool.value;	
}
scsConfigHandle(WheelSimulated) {
	if (current->index >= MAX_WHEEL_COUNT) return;
	telemPtr->tel_rev4.wheelSimulated[current->index] = current->value.value_bool.value;
}
scsConfigHandle(WheelRadius) {
	if (current->index >= MAX_WHEEL_COUNT) return;
	telemPtr->tel_rev4.wheelRadius[current->index] = current->value.value_float.value;
}
scsConfigHandle(WheelPowered) {
	if (current->index >= MAX_WHEEL_COUNT) return;
	telemPtr->tel_rev4.wheelPowered[current->index] = current->value.value_bool.value;
}
scsConfigHandle(WheelLiftable) {
	if (current->index >= MAX_WHEEL_COUNT) return;
	telemPtr->tel_rev4.wheelLiftable[current->index] = current->value.value_bool.value;
}

scsConfigHandle(SelectorCount) { telemPtr->tel_rev4.selectorCount = current->value.value_u32.value; }
scsConfigHandle(SlotGear) {
	if (current->index >= MAX_SLOT_COUNT) return;
	telemPtr->tel_rev4.slotGear[current->index] = current->value.value_s32.value;
}
scsConfigHandle(SlotHandlePosition) {
	if (current->index >= MAX_SLOT_COUNT) return;
	telemPtr->tel_rev4.slotHandlePosition[current->index] = current->value.value_u32.value;
}
scsConfigHandle(SlotSelectors) {
	if (current->index >= MAX_SLOT_COUNT) return;
	telemPtr->tel_rev4.slotSelectors[current->index] = current->value.value_u32.value;
}*/

scsConfigHandle(ShifterType) {	
	strncpy(telemPtr->tel_rev4.shifterType, current->value.value_string.value, MAX_SHIFTER_TYPE_STRING_SIZE);
}