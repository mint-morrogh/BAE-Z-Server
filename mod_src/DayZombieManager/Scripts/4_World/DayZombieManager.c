/**
 * DayZombieManager - Blood & Barter
 *
 * Reduces zombie population during daytime by despawning a percentage
 * of zombies as they spawn. At night, all zombies spawn normally.
 *
 * The Central Economy keeps trying to maintain its target counts,
 * but this mod continuously thins the herd during daylight hours,
 * resulting in roughly half the normal zombie population during the day.
 */

modded class ZombieBase
{
	// Probability that a zombie despawns during daytime (0.0 to 1.0)
	// 0.65 = roughly 35% of zombies survive during day
	static const float DAY_DESPAWN_CHANCE = 0.65;

	// Daytime window (24h format) - outside this range is "night"
	static const int DAY_START_HOUR = 6;
	static const int DAY_END_HOUR = 20;

	override void EEInit()
	{
		super.EEInit();

		if (!GetGame().IsServer())
			return;

		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);

		if (hour >= DAY_START_HOUR && hour < DAY_END_HOUR)
		{
			if (Math.RandomFloat01() < DAY_DESPAWN_CHANCE)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredDespawn, 150, false);
			}
		}
	}

	private void DeferredDespawn()
	{
		if (this)
		{
			GetGame().ObjectDelete(this);
		}
	}
}
