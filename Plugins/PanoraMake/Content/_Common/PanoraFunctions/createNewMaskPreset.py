import unreal

instance = Location
unreal_class = unreal.MaterialEditingLibrary

#asset_tools = unreal.BlueprintFunctionLibrary.get_asset_tools()

#my_new_asset = asset_tools.create_asset_with_dialog(asset_name, package_path, unreal_class, factory)

#unreal.EditorAssetLibrary.save_loaded_asset(my_new_asset)

unreal.MaterialEditingLibrary.update_material_instance(instance)

print("instance")

#unreal.MaterialInterface.get_editor_property(my_new_asset, parent) 

#unreal.MaterialInstance.get_editor_property(my_new_asset, unreal.MaterialInstance.parent(parentmat), notify_mode=PropertyAccessChangeNotifyMode.DEFAULT)