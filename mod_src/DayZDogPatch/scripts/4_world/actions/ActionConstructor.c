modded class ActionConstructor 
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);
		
		actions.Insert(BBP_FollowMe);
        actions.Insert(BBP_StayHere);
        //actions.Insert(ActionDogWhistle);
        actions.Insert(BBP_Patrol);
        //actions.Insert(ActionLickWound);
		actions.Insert(ActionSummonDog);
		actions.Insert(ActionOpendogshed_door);
		actions.Insert(ActionClosedogshed_door);
		actions.Insert(ActionSummonDogMenu);
		actions.Insert(ActionFeedDog);
		actions.Insert(ActionBandageDog);
		actions.Insert(ActionPutDogToKennel);
		actions.Insert(ActionTakeDogFromKennel);
		actions.Insert(ActionSetDogMission);
		actions.Insert(ActionControlDog);
		actions.Insert(ActionStopControlDog);
		actions.Insert(ActionManageDog);
    }
}