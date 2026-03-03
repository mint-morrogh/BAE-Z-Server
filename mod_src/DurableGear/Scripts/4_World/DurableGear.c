/**
 * DurableGear - BAE-Z
 *
 * Items self-repair to full health every 2 minutes.
 * Tools, weapons, cooking gear — everything stays pristine.
 * DecreaseHealth is proto native (engine C++) and can't be
 * overridden in script, so we heal instead of blocking damage.
 */

modded class ItemBase
{
	private bool m_DurableGearInit = false;

	override void EEInit()
	{
		super.EEInit();

		if (!GetGame().IsServer())
			return;

		if (m_DurableGearInit)
			return;

		m_DurableGearInit = true;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DurableGearTick, 120000, true);
	}

	void DurableGearTick()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		if (!IsAlive())
			return;

		float maxHp = GetMaxHealth("", "Health");
		float curHp = GetHealth("", "Health");
		if (curHp < maxHp)
			SetHealth("", "Health", maxHp);
	}
}
