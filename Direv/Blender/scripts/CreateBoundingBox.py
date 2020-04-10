import bpy

bpy.ops.object.origin_set(type = "ORIGIN_GEOMETRY", center = "BOUNDS")
bpy.data.objects["Main"].location = 0,0,0

bpy.ops.mesh.primitive_cube_add(location=(0,0,0))
bpy.context.active_object.name = "Bounding Box"
bpy.data.objects["Bounding Box"].dimensions = bpy.data.objects["Main"].dimensions
