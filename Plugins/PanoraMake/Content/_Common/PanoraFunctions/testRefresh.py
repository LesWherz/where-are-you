import unreal

unreal_class = unreal.MaterialEditingLibrary
unreal_class = unreal.MaterialInstanceConstant
unreal_class = unreal.AssetData(object_path="

-

"', package_name="

-

", package_path="

-

", asset_name="

-

", asset_class="

-

")

instance = unreal.MaterialInstanceConstant.asset_import_data(
unreal.MaterialEditingLibrary.update_material_instance(instance)

print(instance)