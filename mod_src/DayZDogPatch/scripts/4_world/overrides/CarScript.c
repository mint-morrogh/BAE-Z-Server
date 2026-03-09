modded class CarScript
{
	override bool CanBeTargetedByAI( EntityAI ai )
	{
		Dayz_Doggo dog = Dayz_Doggo.Cast(ai);
		if (dog)
		{
			return false;
		}
		return super.CanBeTargetedByAI(ai);
	}
};