modded class DayZPlayerImplementThrowing
{

	override void HandleThrowing(HumanInputController pHic, HumanCommandWeapons pHcw, EntityAI pEntityInHands, float pDt)
	{
		super.HandleThrowing(pHic, pHcw, pEntityInHands, pDt);
		dog_bone_toy toybone = dog_bone_toy.Cast(pEntityInHands);
		if (toybone)
		{
			toybone.SetToyThrower(PlayerBase.Cast(m_Player));
		}
	}
}