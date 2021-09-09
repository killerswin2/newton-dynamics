# Copyright (c) <2003-2021> <Newton Game Dynamics>
 
# This software is provided 'as-is', without any express or implied
# warranty. In no event will the authors be held liable for any damages
# arising from the use of this software.
 
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely

bl_info = {
	"name": "Newton Physics",
	"category": "Physics",
	"author": "Newton Dynamics",
	"version": (1, 0, 0),
	"blender": (2, 93, 0),
	#"location": "Properties > Scene > Blend Info panel",
    #"location": "3D Viewport > Sidebar > Item tab",
    "location": "View3D",
	"description": "Add Newton Physics to meshes",
}

import os
import sys
import bpy

# this should works, but it fail 
# so I have to add a enviroment variable to set the path blender path
#blenderPath = 'scripts/addons/newtonPy'
blenderPath = os.getenv('Blender') + '/scripts/addons/newtonPy'
#print (blenderPath)

if not blenderPath in sys.path:
	sys.path.append(blenderPath)


import newtonWorld

class NewtonWorldPanel(bpy.types.Panel):
    bl_label = "Newton Physics"
    bl_idname = "SCENE_PT_layout"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "newton"

    def findHome(self, scene):
        col = bpy.data.collections.get("Collection")
        if col:
            for obj in col.objects:
                if obj.name == 'newtonHome':
                    return obj
        return None
    
    def draw(self, context):
        scene = context.scene
        layout = self.layout
        
        world = scene.newton_world
        if world is None:
            newtonHome = self.findHome(scene)
            if newtonHome is None:
                layout.operator("view3d.newton_world_create_home")
                #newtonHome = self.findHome(scene)
            else:
                layout.operator("view3d.newton_world_create")
        else:
            layout.operator("view3d.newton_world_destroy")

            #list engine parameters
            layout.label(text="Engine Configuration")
            row = layout.row()
            row.prop(world, "name")
            #row.prop(scene, "substeps")
            #row.prop(world, "iterations")
            #row.alignment = 'RIGHT'


classes = [
    NewtonWorldPanel,
    newtonWorld.NewtonWorldCreate,
    newtonWorld.NewtonWorldDestroy,
    newtonWorld.NewtonWorldCreateHomeObject
]

def register():
    for cls in classes:
        bpy.utils.register_class(cls)

    bpy.types.Scene.newton_world = bpy.props.PointerProperty(
		type = bpy.types.Object,
		name = "newton world",)


def unregister():
    for cls in classes:
        bpy.utils.unregister_class(cls)

if __name__ == "__main__":
    register()
