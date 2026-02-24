/**
 * CampfireRegen - Blood & Barter
 *
 * Players near a lit fire slowly regenerate health and blood.
 * +2 health and +5 blood every 10 seconds within 5m of a burning fire.
 */

modded class FireplaceBase
{
	private bool m_RegenTimerStarted = false;

	override protected void SetBurningState(bool is_burning)
	{
		super.SetBurningState(is_burning);

		if (!GetGame().IsServer())
			return;

		if (is_burning)
		{
			if (!m_RegenTimerStarted)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CampfireRegenTick, 10000, true);
				m_RegenTimerStarted = true;
			}
		}
		else
		{
			if (m_RegenTimerStarted)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CampfireRegenTick);
				m_RegenTimerStarted = false;
			}
		}
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame() && GetGame().IsServer() && m_RegenTimerStarted)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CampfireRegenTick);
		}

		super.EEDelete(parent);
	}

	private void CampfireRegenTick()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		if (!IsBurning())
			return;

		vector pos = GetPosition();

		array<Object> nearObjects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition(pos, 5.0, nearObjects, proxyCargos);

		for (int i = 0; i < nearObjects.Count(); i++)
		{
			PlayerBase player = PlayerBase.Cast(nearObjects.Get(i));
			if (!player)
				continue;

			if (!player.IsAlive())
				continue;

			float curHealth = player.GetHealth("GlobalHealth", "Health");
			if (curHealth < 100.0)
			{
				float newHealth = curHealth + 2.0;
				if (newHealth > 100.0)
					newHealth = 100.0;
				player.SetHealth("GlobalHealth", "Health", newHealth);
			}

			float curBlood = player.GetHealth("GlobalHealth", "Blood");
			if (curBlood < 5000.0)
			{
				float newBlood = curBlood + 5.0;
				if (newBlood > 5000.0)
					newBlood = 5000.0;
				player.SetHealth("GlobalHealth", "Blood", newBlood);
			}
		}
	}
}
