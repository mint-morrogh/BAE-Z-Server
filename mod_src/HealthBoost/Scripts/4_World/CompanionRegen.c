/**
 * CompanionRegen - BAE-Z (HealthBoost)
 *
 * Dogs: 10% max health every 15 seconds (~2.5 min near-death to full).
 * Horses: 5% max health every 30 seconds (~10 min near-death to full).
 * Only affects entities with "Doggo" or "Horse" in their class name.
 */

modded class AnimalBase
{
	private bool m_CompanionRegenStarted = false;

	override void EEInit()
	{
		super.EEInit();

		if (!GetGame().IsServer())
			return;

		if (m_CompanionRegenStarted)
			return;

		if (IsDog())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DogRegenTick, 15000, true);
			m_CompanionRegenStarted = true;
		}
		else if (IsHorse())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(HorseRegenTick, 30000, true);
			m_CompanionRegenStarted = true;
		}
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame() && GetGame().IsServer() && m_CompanionRegenStarted)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(DogRegenTick);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(HorseRegenTick);
		}

		super.EEDelete(parent);
	}

	private bool IsDog()
	{
		string className = GetType();
		className.ToLower();
		return className.Contains("doggo");
	}

	private bool IsHorse()
	{
		string className = GetType();
		className.ToLower();
		return className.Contains("horse");
	}

	private void DogRegenTick()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		if (!IsAlive())
			return;

		float maxHealth = GetMaxHealth("GlobalHealth", "Health");
		float curHealth = GetHealth("GlobalHealth", "Health");

		if (curHealth < maxHealth)
		{
			float newHealth = curHealth + (maxHealth * 0.10);
			if (newHealth > maxHealth)
				newHealth = maxHealth;
			SetHealth("GlobalHealth", "Health", newHealth);
		}
	}

	private void HorseRegenTick()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		if (!IsAlive())
			return;

		float maxHealth = GetMaxHealth("GlobalHealth", "Health");
		float curHealth = GetHealth("GlobalHealth", "Health");

		if (curHealth < maxHealth)
		{
			float newHealth = curHealth + (maxHealth * 0.05);
			if (newHealth > maxHealth)
				newHealth = maxHealth;
			SetHealth("GlobalHealth", "Health", newHealth);
		}
	}
}
