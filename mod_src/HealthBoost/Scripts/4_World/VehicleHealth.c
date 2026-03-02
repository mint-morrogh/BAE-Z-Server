/**
 * VehicleHealth - Blood & Barter (HealthBoost)
 *
 * Every 60 seconds, vehicle global health and all damage zones
 * (hood, bumpers, doors, wheels, engine, etc.) are repaired to max.
 * Ruined parts (0 HP) are left alone — they need replacement.
 * Fluids and gas still need managing.
 */

modded class CarScript
{
	private bool m_VehicleRepairStarted = false;

	override void EEInit()
	{
		super.EEInit();

		if (!GetGame().IsServer())
			return;

		if (m_VehicleRepairStarted)
			return;

		m_VehicleRepairStarted = true;
		VehicleRepairTick();
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(VehicleRepairTick, 60000, true);
	}

	void VehicleRepairTick()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		if (!IsAlive())
			return;

		// Global chassis health
		float maxHp = GetMaxHealth("", "Health");
		float curHp = GetHealth("", "Health");
		if (curHp < maxHp)
			SetHealth("", "Health", maxHp);

		// Repair all individual damage zones (hood, doors, bumpers, wheels, engine, etc.)
		TStringArray zones = new TStringArray;
		GetDamageZones(zones);
		for (int i = 0; i < zones.Count(); i++)
		{
			float zoneHp = GetHealth(zones[i], "Health");
			if (zoneHp <= 0)
				continue; // ruined — needs replacement

			float zoneMax = GetMaxHealth(zones[i], "Health");
			if (zoneHp < zoneMax)
				SetHealth(zones[i], "Health", zoneMax);
		}
	}
}
