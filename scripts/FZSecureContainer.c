class MySecureContainerItem : ItemBase
{
    // Prevent the item from being moved to the player's inventory or cargo of another item
    override bool CanPutInCargo(EntityAI parent)
    {
        if (parent.GetIdentity().GetId() == GetInventoryOwner().GetIdentity().GetId())
        {
            return true;
        }
        return false;
    }

    override bool CanPutIntoHands(EntityAI player)
    {
        if (player.GetIdentity().GetId() == GetInventoryOwner().GetIdentity().GetId())
        {
            return true;
        }
        return false;
    }

    // Allow the player to remove items from this item's cargo
    override bool CanRemoveFromCargo(EntityAI item)
    {
        if (item.GetInventoryOwner().GetIdentity().GetId() == GetInventoryOwner().GetIdentity().GetId())
        {
            return true;
        }
        return false;
    }

    // Allow the player to put items into this item's cargo
    override bool CanReceiveItemIntoCargo(EntityAI item)
    {
        if (item.GetInventoryOwner().GetIdentity().GetId() == GetInventoryOwner().GetIdentity().GetId())
        {
            return true;
        }
        return false;
    }

    // Define cargo size
    override int GetInventorySlotCount()
    {
        return 9; // 3x3 slot capacity
    }

    // When a player acquires a MySecureContainerItem
    void OnAcquireContainer(PlayerBase player, MySecureContainerItem container)
    {
        // Get the unique IDs
        string playerId = player.GetIdentity().GetId();
        string containerId = GetContainerId(container); // You'll need to implement a method to get or assign unique IDs to containers

        // Serialize the container's contents. This depends on your setup and could be as simple as a list of item type names or as complex as a custom serialization format.
        string contents = SerializeContainerContents(container);

        // Store the data in the database using a CF database call (the actual syntax will depend on your setup)
        CF.Database.Execute("INSERT INTO containers (playerId, containerId, contents) VALUES (?, ?, ?)", {playerId, containerId, contents});
    }

    // When a player respawns or reconnects
    void OnPlayerSpawn(PlayerBase player)
    {
        // Get the player's unique ID
        string playerId = player.GetIdentity().GetId();

        // Retrieve the player's container data from the database
        array<ref DatabaseRow> results = CF.Database.Query("SELECT * FROM containers WHERE playerId = ?", playerId);

        // If a container was found
        if (results.Count() > 0)
        {
            // Create the container in the player's inventory
            MySecureContainerItem container = player.GetInventory().CreateInInventory("MySecureContainerItem");

            // Deserialize the contents and add them to the container
            DeserializeContainerContents(container, results[0].GetValue("contents"));
        }
    }
};
