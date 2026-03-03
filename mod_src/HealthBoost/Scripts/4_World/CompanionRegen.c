/**
 * CompanionRegen - BAE-Z (HealthBoost)
 *
 * Dogs and horses passively regenerate 5% of max health every 30 seconds.
 * Near-death to full health in ~10 minutes. Only affects entities with
 * "Doggo" or "Horse" in their class name (wolves, deer, etc. unaffected).
 */

modded class AnimalBase
{
	private bool m_CompanionRegenStarted = false;

	override void EEInit()
	{
		super.EEInit();

		if (!GetGame().IsServer())
			return;

		if (!IsCompanion())
			return;

		if (!m_CompanionRegenStarted)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CompanionRegenTick, 30000, true);
			m_CompanionRegenStarted = true;
		}
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame() && GetGame().IsServer() && m_CompanionRegenStarted)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CompanionRegenTick);
		}

		super.EEDelete(parent);
	}

	private bool IsCompanion()
	{
		string className = GetType();
		className.ToLower();
		return (className.Contains("doggo") || className.Contains("horse"));
	}

	private void CompanionRegenTick()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		if (!IsAlive())
			return;

		float maxHealth = GetMaxHealth("GlobalHealth", "Health");
		float curHealth = GetHealth("GlobalHealth", "Health");

		if (curHealth < maxHealth)
		{
			float regenAmount = maxHealth * 0.05;
			float newHealth = curHealth + regenAmount;
			if (newHealth > maxHealth)
				newHealth = maxHealth;
			SetHealth("GlobalHealth", "Health", newHealth);
		}
	}
}
