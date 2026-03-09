modded class PluginRecipesManager 
{
   override void RegisterRecipies()
    {
        super.RegisterRecipies();
        
        RegisterRecipe(new dismantle_dog_shed_big);
		RegisterRecipe(new dismantle_dog_shed_wooden);
		RegisterRecipe(new dismantle_dog_shed_small);
    }
};