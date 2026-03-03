/**
 * SitRest - BAE-Z
 *
 * Freezes hunger (energy) and thirst (water) drain while the player
 * is using a sit emote (SitA, SitB, or SurvivorAnims SitNew). Eating/drinking while seated
 * still works — the snapshot ratchets upward.
 */

modded class EmoteManager
{
	int SitRest_GetGestureID()
	{
		return m_CurrentGestureID;
	}
}

modded class PlayerBase
{
	private bool m_SitRest_IsSitting = false;
	private bool m_SitRest_TimerStarted = false;
	private float m_SitRest_Energy = -1;
	private float m_SitRest_Water = -1;

	override void Init()
	{
		super.Init();

		if (!GetGame().IsServer())
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SitRestTick, 1000, true);
		m_SitRest_TimerStarted = true;
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame() && GetGame().IsServer() && m_SitRest_TimerStarted)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(SitRestTick);
		}

		super.EEDelete(parent);
	}

	private bool SitRest_IsSitEmoteActive()
	{
		EmoteManager em = GetEmoteManager();
		if (!em)
			return false;

		int id = em.SitRest_GetGestureID();
		// Vanilla sits + SurvivorAnims "SitNew" (ID 5501)
		return (id == EmoteConstants.ID_EMOTE_SITA || id == EmoteConstants.ID_EMOTE_SITB || id == 5501);
	}

	private void SitRestTick()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		if (!IsAlive())
			return;

		bool sitting = SitRest_IsSitEmoteActive();

		if (sitting && !m_SitRest_IsSitting)
		{
			// Just sat down — snapshot current stats
			m_SitRest_Energy = GetStatEnergy().Get();
			m_SitRest_Water = GetStatWater().Get();
			m_SitRest_IsSitting = true;
		}
		else if (sitting && m_SitRest_IsSitting)
		{
			// Still sitting — freeze drain, allow eating/drinking
			float curEnergy = GetStatEnergy().Get();
			float curWater = GetStatWater().Get();

			if (curEnergy > m_SitRest_Energy)
				m_SitRest_Energy = curEnergy;
			else if (curEnergy < m_SitRest_Energy)
				GetStatEnergy().Set(m_SitRest_Energy);

			if (curWater > m_SitRest_Water)
				m_SitRest_Water = curWater;
			else if (curWater < m_SitRest_Water)
				GetStatWater().Set(m_SitRest_Water);
		}
		else if (!sitting && m_SitRest_IsSitting)
		{
			// Stood up — clear snapshot, resume normal drain
			m_SitRest_IsSitting = false;
			m_SitRest_Energy = -1;
			m_SitRest_Water = -1;
		}
	}
}
