bl_info = {
    "name": "Terrarium Generator",
    "author": "shirover",
    "version": (1, 0),
    "blender": (2, 80, 0),
    "location": "View3D > Add > Mesh > New Object",
    "description": "Adds a new Mesh Object",
    "warning": "",
    "doc_url": "",
    "category": "Add Mesh",
}


import bpy, mathutils
from bpy.types import Panel, Operator
from bpy.props import FloatProperty
from mathutils import Matrix, Vector
import os
import glob
from re import search
import math




#===================== CLASSES ================================
class AddonMainPanel(bpy.types.Panel):
    """Creates a Panel in the Object properties window"""
    bl_label = "Terrarium Generator"    # 
    bl_idname = "OBJECT_PT_TerrGen"     # id
    bl_space_type = 'VIEW_3D'           # space where the header is going to be used in(VIEW_3D = 3D Viewport)
    bl_region_type = 'UI'               # region where the header is going to be used in
    bl_category = 'Terrarium Generator' # Panel name which will be show 

    def draw(self, context):

        layout = self.layout
        row = layout.row()
        row.label(text= "Add an terrarium", icon= 'CUBE')
        # Add button which will be add base meshes
        row = layout.row()
        row.operator("wm.addop") 
 
        # Add resize sliderPanel
        row = layout.row()
        row.template_ID(context.view_layer.objects, "active")

        split=layout.split()
        col=split.column(align=True)
        obj = context.object
        substring = "errarium"
        
        # check if active object is terrarium; if yes so we can resize it
        if obj is not None and search(substring, obj.name):  
            col.prop(obj, 'myFloatX', slider=True)
            col.prop(obj, 'myFloatY', slider=True)
            col.prop(obj, 'myFloatZ', slider=True)
        
class BaseMeshesAdder(bpy.types.Operator):
    """Operator responsible for importing base meshes into the scene, 
       adding base objects"""
    bl_label = "Add"                # Button Name
    bl_idname = "wm.addop"          # Window Manager Add Operator   
    
    def execute(self, context):
        # getting the path to the folder with the base meshes 
        mesh_directory = get_dir()
        files = glob.glob(mesh_directory + "*.fbx")

        # import base meshes to scene
        for f in files:       
            bpy.ops.import_scene.fbx(filepath=f)

        
        add_bowl(self, context)
        
        ## call function that add empty objects and rock wall
        
        add_empty(self, context)       
        add_rock_wall(self, context)
        add_log(self, context)

        return {'FINISHED'}
#==============================================================




#==================== RESIZE TERRARIUM ========================
def set_location(self, value, idx):
    len_xyz = value    

    obj = bpy.context.active_object
    num_of_vertices = len(obj.data.vertices)
    
    bpy.ops.object.mode_set(mode = 'EDIT') 
    bpy.ops.mesh.select_mode(type="VERT")
    bpy.ops.mesh.select_all(action = 'DESELECT')
    bpy.ops.object.mode_set(mode = 'OBJECT')

    START_SIZE = obj.data.vertices[0].co[idx]
    
    len_xyz = START_SIZE + len_xyz
    poz_x = obj.data.vertices[4].co[idx]
    poz_y = obj.data.vertices[2].co[idx]
    poz_z = obj.data.vertices[1].co[idx]
    
    for i in range(num_of_vertices):
        if obj.data.vertices[i].co[idx] > 0:
            dif = 0
            if idx == 0 and obj.data.vertices[i].co[idx] < poz_x:
                dif = poz_x - obj.data.vertices[i].co[idx]
            elif idx == 1 and obj.data.vertices[i].co[idx] < poz_y:
                dif = poz_y - obj.data.vertices[i].co[idx]
            elif idx == 2 and obj.data.vertices[i].co[idx] < poz_z:
                dif = poz_z - obj.data.vertices[i].co[idx]
            obj.data.vertices[i].co[idx] = len_xyz - dif
            
            
    bpy.ops.object.origin_set(type='ORIGIN_GEOMETRY', center='MEDIAN')
    bpy.ops.view3d.snap_selected_to_cursor(use_offset=True)


    bpy.ops.object.mode_set(mode = 'EDIT')

    #rock wall  
    bpy.data.objects["Empty_1"].location = obj.data.vertices[2].co
    bpy.data.objects["Empty_2"].location = obj.data.vertices[3].co
    bpy.data.objects["Empty_3"].location = obj.data.vertices[6].co
    bpy.data.objects["Empty_4"].location = obj.data.vertices[7].co

    #bowl
    bpy.data.objects["Empty_5"].location = obj.data.vertices[4].co

    #log
    bpy.data.objects["Empty_6"].location = obj.data.vertices[0].co


#get, set methods of the floatproperty
def get_locationX(self):
    return self.get('X', bpy.context.object.dimensions.x)

def get_locationY(self):
    return self.get('Y', bpy.context.object.dimensions.y)

def get_locationZ(self):
    return self.get('Z', bpy.context.object.dimensions.z)


def set_locationX(self, value):
    set_location(self, value, 0)

def set_locationY(self, value):
    set_location(self, value, 1)

def set_locationZ(self, value):
    set_location(self, value, 2)


#floatproperty
bpy.types.Object.myFloatX = FloatProperty(
name = "X", 
description = "Set the size on local x axis",
min = 0.5,
max = 100,
soft_min=0.5,
soft_max=10,
step=1,
subtype='DISTANCE',
get = get_locationX,
set = set_locationX)

bpy.types.Object.myFloatY = FloatProperty(
name = "Y", 
description = "Set the size on local y axis",
min = 0.5,
max = 100,
soft_min=0.5,
soft_max=10,
step=1,
subtype='DISTANCE',
get = get_locationY,
set = set_locationY)

bpy.types.Object.myFloatZ = FloatProperty(
name = "Z", 
description = "Set the size on local z axis",
min = 0.5,
max = 100,
soft_min=0.5,
soft_max=10,
step=1,
subtype='DISTANCE',
get = get_locationZ,
set = set_locationZ)
#==============================================================




#==================== IMPORT ==================================
def get_dir():
    """ Return path to folder with base meshes """

    absolute_path = os.path.dirname(os.path.abspath(__file__))
    mesh_dir, tail = os.path.split(absolute_path)
    mesh_dir = mesh_dir + "\\BaseAddonMeshes\\"
    return mesh_dir
#==============================================================




#==================== ROCK WALL ===============================
def add_rock_wall(self, context):
    # creating variables
    name = "Rock wall"
    _location = (0, 0.15, 0)
    _scale = (8.5, 3, 8.5)
    _value = (0.026, 0.026, 0.026)
    # crease 
    _edge_idx = [0, 2, 3, 4, 5, 6, 7, 8, 10]
    # deform vertex group
    _vertex_group_data = [0, 1, 4, 5]
    # subdivision params
    _subdiv_lvl = 6
    # displace texture params
    _tex_color = (0.867, 0.867, 0.867, 0)
    _colpram_el_pos = 0.243


    bpy.ops.mesh.primitive_cube_add(location=_location, scale=_scale)
    
    #RENAME OBJECT
    bpy.data.objects["Cube"].name = name
    
    # SELECT EDGES AND INCREASE CREASE
    obj = bpy.data.objects[name]
    
    bpy.ops.object.editmode_toggle()
    bpy.ops.mesh.select_mode(type="EDGE")
    bpy.ops.mesh.select_all(action = 'DESELECT')
    bpy.ops.object.mode_set(mode = 'OBJECT')
    for i in _edge_idx:
        obj.data.edges[i].select = True
    bpy.ops.object.mode_set(mode = 'EDIT') 
    bpy.ops.transform.edge_crease(value=1.0)
    bpy.ops.object.mode_set(mode = 'OBJECT')


    obj = bpy.ops.object
    
    bpy.context.object.data.use_auto_smooth = True
    bpy.context.space_data.shading.light = 'MATCAP'
    bpy.context.space_data.shading.studio_light = 'clay_studio.exr'
    bpy.context.space_data.shading.show_cavity = True
    
    #create vertex group
    new_vertex_group = bpy.context.object.vertex_groups.new(name='DeformGroup')
    new_vertex_group.add(_vertex_group_data, 1.0, 'ADD')

    # HOOKS (4 times)
    for i in range(0,4):
        bpy.ops.object.modifier_add(type='HOOK')
        bpy.context.object.modifiers["Hook"].name = "Hook" + str(i+1)

    # SUBDIVISION
    obj.modifier_add(type='SUBSURF')
    bpy.context.object.modifiers["Subdivision"].levels = _subdiv_lvl
    
    #BASE DISPLACEMENT
    obj.modifier_add(type='DISPLACE')
    bpy.context.object.modifiers["Displace"].name = "BaseDisplacement"
    #set texture params
    tex_1 = bpy.data.textures.new("BaseDisplacementNoise", 'MUSGRAVE')
    tex_1.noise_basis = 'VORONOI_F2_F1'
    tex_1.noise_scale = 4
    tex_1.use_color_ramp = True
    tex_1.color_ramp.elements.new(_colpram_el_pos)
    for i in tex_1.color_ramp.elements:
        i.color[3] = 0
    tex_1.color_ramp.elements[1].color = _tex_color
    #add texture to modifier
    bpy.context.object.modifiers["BaseDisplacement"].texture = bpy.data.textures['BaseDisplacementNoise']
    bpy.context.object.modifiers["BaseDisplacement"].mid_level = 0
    bpy.context.object.modifiers["BaseDisplacement"].strength = 0.7
    bpy.context.object.modifiers["BaseDisplacement"].texture_coords = "LOCAL"
    bpy.context.object.modifiers["BaseDisplacement"].vertex_group = "DeformGroup"
   
    #SECONDARY DISPLACEMENT
    obj.modifier_add(type='DISPLACE')
    bpy.context.object.modifiers["Displace"].name = "SecondaryDisplacement"
    #set texture params
    tex_2 = bpy.data.textures.new("SecondaryDisplacementNoise", 'MUSGRAVE')
    tex_2.noise_basis = 'VORONOI_F2_F1'
    tex_2.noise_scale = 3
    tex_2.use_color_ramp = True
    tex_2.color_ramp.elements.new(_colpram_el_pos)
    for j in tex_2.color_ramp.elements:
        j.color[3] = 0
    tex_2.color_ramp.elements[1].color = _tex_color       
    #add texture to modifier
    bpy.context.object.modifiers["SecondaryDisplacement"].texture = bpy.data.textures['SecondaryDisplacementNoise']
    bpy.context.object.modifiers["SecondaryDisplacement"].mid_level = 0.1
    bpy.context.object.modifiers["SecondaryDisplacement"].strength = 0.5
    bpy.context.object.modifiers["SecondaryDisplacement"].texture_coords = 'GLOBAL'
    bpy.context.object.modifiers["SecondaryDisplacement"].vertex_group = "DeformGroup"
   
    #1ST DECIMATE
    bpy.ops.object.modifier_add(type='DECIMATE')
    bpy.context.object.modifiers["Decimate"].name = "Decimate_1st"
    bpy.context.object.modifiers["Decimate_1st"].decimate_type = 'COLLAPSE'
    bpy.context.object.modifiers["Decimate_1st"].ratio = 0.1
    
    #2ND DECIMATE
    bpy.ops.object.modifier_add(type='DECIMATE')
    bpy.context.object.modifiers["Decimate"].name = "Decimate_2nd"
    bpy.context.object.modifiers["Decimate_2nd"].decimate_type = 'DISSOLVE'
    bpy.context.object.modifiers["Decimate_2nd"].angle_limit = 0.261799
    bpy.context.object.modifiers["Decimate_2nd"].use_dissolve_boundaries = True

    #SMOOTH
    bpy.ops.object.modifier_add(type='SMOOTH')
    bpy.context.object.modifiers["Smooth"].iterations = 4
    bpy.context.object.modifiers["Smooth"].vertex_group = "DeformGroup"

    #BEVEL
    bpy.ops.object.modifier_add(type='BEVEL')
    bpy.context.object.modifiers["Bevel"].limit_method = 'ANGLE'
    bpy.context.object.modifiers["Bevel"].angle_limit = 0.349066
    bpy.context.object.modifiers["Bevel"].offset_type = 'PERCENT'
    bpy.context.object.modifiers["Bevel"].width_pct = 10
    bpy.context.object.modifiers["Bevel"].use_clamp_overlap = False

    #WEIGHTED NORMAL
    bpy.ops.object.modifier_add(type='WEIGHTED_NORMAL')
    
    #TRIANGULATE
    bpy.ops.object.modifier_add(type='TRIANGULATE')
    bpy.context.object.modifiers["Triangulate"].keep_custom_normals = True

    # RESIZE
    bpy.ops.transform.resize(value=_value)

    # adding parent to empty objects
    add_parent(self, context)
    
    # adding more loop cuts for correct work of rock textures
    bpy.ops.object.mode_set(mode = 'EDIT') 
    bpy.ops.mesh.select_all(action='SELECT')
    bpy.ops.mesh.subdivide(number_cuts=2)
    bpy.ops.object.mode_set(mode = 'OBJECT')    

def add_empty(self, context):
    # adding 4 empty objects in scene and renaming them
    for i in range(0,4):
        name = "Empty_" + str(i+1)
        bpy.ops.object.empty_add(type='PLAIN_AXES', radius=0.3, align='WORLD', location=(0, 0, 0), scale=(1, 1, 1))
        bpy.data.objects["Empty"].name = name

    # setting empty objects location
    bpy.data.objects["Empty_1"].location = (-0.25, 0.25, -0.25)
    bpy.data.objects["Empty_2"].location = (-0.25, 0.25,  0.25)
    bpy.data.objects["Empty_3"].location = ( 0.25, 0.25, -0.25)
    bpy.data.objects["Empty_4"].location = ( 0.25, 0.25,  0.25)

def add_parent(self, context):
    name = "Rock wall" 
    # deform vertex group
    _vertex_group_data_1 = [0, 2]
    _vertex_group_data_2 = [1, 3]
    _vertex_group_data_3 = [4, 6]
    _vertex_group_data_4 = [5, 7]
    
    obj = bpy.data.objects[name]  
    # assign vertices to vertex groups
    # 1
    new_vertex_group_1 = bpy.context.object.vertex_groups.new(name='1_corner')
    new_vertex_group_1.add(_vertex_group_data_1, 1.0, 'ADD')
    # 2
    new_vertex_group_2 = bpy.context.object.vertex_groups.new(name='2_corner')
    new_vertex_group_2.add(_vertex_group_data_2, 1.0, 'ADD')
    # 3
    new_vertex_group_3 = bpy.context.object.vertex_groups.new(name='3_corner')
    new_vertex_group_3.add(_vertex_group_data_3, 1.0, 'ADD')
    # 4
    new_vertex_group_4 = bpy.context.object.vertex_groups.new(name='4_corner')
    new_vertex_group_4.add(_vertex_group_data_4, 1.0, 'ADD')
    
    # adding vertex groups to modifiers
    for i in range(0,4):
        bpy.context.object.modifiers["Hook" + str(i+1)].object = bpy.data.objects["Empty_" + str(i+1)]
        bpy.context.object.modifiers["Hook" + str(i+1)].vertex_group = str(i+1) + "_corner"
#==============================================================




#==================== GROUND ==================================

#==============================================================




#==================== WATER BOWL ==============================
def add_bowl(self, context):
    ob = bpy.context.scene.objects["bowl"]       # Get the object
    bpy.ops.object.select_all(action='DESELECT')    # Deselect all objects
    bpy.context.view_layer.objects.active = ob      # Make the drinker the active object 
    ob.select_set(True)                             # Select the drinker
    ob.location = (0.16, -0.17, -0.23)
    bowl_modifiers()

    bpy.ops.object.empty_add(type='PLAIN_AXES', 
                             radius=0.3, 
                             align='WORLD', 
                             location=(0.25, -0.25, -0.23), 
                             scale=(1, 1, 1))
    bpy.data.objects["Empty"].name = "Empty_5"  
    bpy.data.objects["Empty_5"].select_set(True) 

    bpy.data.objects["bowl"].parent = bpy.data.objects["Empty_5"]
    # have to set the inverse matrix of the child to clear the initial transformation of the parent at   parenting moment
    bpy.data.objects["bowl"].matrix_parent_inverse = bpy.data.objects["Empty_5"].matrix_world.inverted()
    
def bowl_modifiers():
    bpy.ops.object.modifier_add(type='BEVEL')
    # bpy.context.scene.objects["drinker"]
    bpy.context.object.modifiers["Bevel"].affect = 'EDGES'
    bpy.context.object.modifiers["Bevel"].width = 0.002
    bpy.context.object.modifiers["Bevel"].segments = 2
#==============================================================




#==================== LOG =====================================
def add_log(self, context):
    name = "log"

    # add empty
    bpy.ops.object.empty_add(type='PLAIN_AXES', radius=0.3, align='WORLD', location=(-0.25, -0.25, -0.23), scale=(1, 1, 1))
    bpy.data.objects["Empty"].name = "Empty_6" 

    add_curve()

    # 1st modifier
    bpy.ops.object.modifier_add(type='HOOK')
    bpy.context.object.modifiers["Hook"].name = "Hook_1"
    bpy.context.object.modifiers["Hook_1"].object = bpy.data.objects["Empty_6"]

    # 2nd modifier
    bpy.ops.object.modifier_add(type='HOOK')
    bpy.context.object.modifiers["Hook"].name = "Hook_2"
    bpy.context.object.modifiers["Hook_2"].object = bpy.data.objects["Empty_4"]    

    #
    bpy.ops.object.mode_set(mode='EDIT')
    bpy.ops.curve.select_all(action = 'DESELECT')

    # for clear code
    p0 = bpy.context.object.data.splines.active.bezier_points[0]
    p1 = bpy.context.object.data.splines.active.bezier_points[1]

    # assign first control point to modifier
    p0.select_control_point = True
    p1.select_control_point = False
    bpy.ops.object.hook_assign(modifier="Hook_1")

    bpy.ops.curve.select_all(action = 'SELECT')
    bpy.ops.curve.select_all(action = 'DESELECT')

    # assign second control point to modifier
    p0.select_control_point = False
    p1.select_control_point = True
    bpy.ops.object.hook_assign(modifier="Hook_2")

    bpy.ops.object.mode_set(mode='OBJECT')

    #add Geometry node moodifier
    log_node_group()

def add_curve():
    name = "log"
    # add curve and set scale / rotation
    bpy.ops.curve.primitive_bezier_curve_add(enter_editmode=False, align='WORLD', location=(0, 0, 0), scale=(1, 1, 1))
    bpy.data.objects["BezierCurve"].name = name
    bpy.data.objects[name].rotation_euler[0] = math.radians(270)
    bpy.data.objects[name].scale = (0.25, 0.25, 0.25)
    bpy.context.object.data.resolution_u = 25
    bpy.ops.object.transform_apply(location=False, rotation=True, scale=True)

    
    # set curve points location
    bpy.ops.object.mode_set(mode='OBJECT')
    point = bpy.context.object.data.splines.active.bezier_points[0]
    point.co = (-0.235, -0.072, -0.185)
    point.handle_left =     (-0.362, -0.067, -0.188)
    point.handle_right =    (-0.110, -0.077, -0.184)

    point = bpy.context.object.data.splines.active.bezier_points[1]
    point.co = (0.222, 0.083, 0.161) 
    point.handle_left =     (0.095, 0.031,  0.248)
    point.handle_right =    (0.349, 0.135,  0.074)
   
def log_node_group():
    # add new geometry node group
    log= bpy.data.node_groups.new(type = "GeometryNodeTree", name = "log")

    # add modifier
    bpy.ops.object.modifier_add(type='NODES')
    bpy.context.object.modifiers["GeometryNodes"].node_group = log

    #log outputs
    log.outputs.new("NodeSocketGeometry", "Geometry")

    #node Group Output
    group_output = log.nodes.new("NodeGroupOutput")
    group_output.location = (1844.169921875, 100.76422119140625)
    group_output.width, group_output.height = 140.0, 100.0


    #log inputs
    #input Geometry
    log.inputs.new("NodeSocketGeometry", "Geometry")


    #node Group Input
    group_input = log.nodes.new("NodeGroupInput")
    group_input.location = (-1809.9521484375, 34.25474166870117)
    group_input.width, group_input.height = 140.0, 100.0

    #node Set Curve Tilt
    set_curve_tilt = log.nodes.new("GeometryNodeSetCurveTilt")
    set_curve_tilt.location = (-432.19610595703125, 60.15542221069336)
    set_curve_tilt.width, set_curve_tilt.height = 140.0, 100.0
    #Selection
    set_curve_tilt.inputs[1].default_value = True
    #Tilt
    set_curve_tilt.inputs[2].default_value = 0.0

    #node Math 1
    math_node_1 = log.nodes.new("ShaderNodeMath")
    math_node_1.location = (-612.4532470703125, -16.62493133544922)
    math_node_1.width, math_node_1.height = 140.0, 100.0
    math_node_1.operation = 'MULTIPLY'
    #Value
    math_node_1.inputs[0].default_value = 0.5
    #Value_001
    math_node_1.inputs[1].default_value = 25.099998474121094
    #Value_002
    math_node_1.inputs[2].default_value = 0.5

    #node Spline Parameter
    spline_parameter = log.nodes.new("GeometryNodeSplineParameter")
    spline_parameter.location = (-791.5067138671875, -77.38319396972656)
    spline_parameter.width, spline_parameter.height = 140.0, 100.0

    #node Set Curve Radius
    set_curve_radius = log.nodes.new("GeometryNodeSetCurveRadius")
    set_curve_radius.location = (201.893310546875, 81.93059539794922)
    set_curve_radius.width, set_curve_radius.height = 140.0, 100.0
    #Selection
    set_curve_radius.inputs[1].default_value = True
    #Radius
    set_curve_radius.inputs[2].default_value = 0.004999999888241291

    #node Float Curve
    float_curve = log.nodes.new("ShaderNodeFloatCurve")
    float_curve.location = (-76.3481216430664, -10.465307235717773)
    float_curve.width, float_curve.height = 240.0, 100.0
    #mapping settings			
    float_curve.mapping.extend = 'EXTRAPOLATED'
    float_curve.mapping.tone = 'STANDARD'
    float_curve.mapping.black_level = (0.0, 0.0, 0.0)
    float_curve.mapping.white_level = (1.0, 1.0, 1.0)
    float_curve.mapping.clip_min_x = 0.0
    float_curve.mapping.clip_min_y = 0.0
    float_curve.mapping.clip_max_x = 1.0
    float_curve.mapping.clip_max_y = 2.0
    float_curve.mapping.use_clip = True
    #curve 0			
    float_curve_0 = float_curve.mapping.curves[0]
    float_curve_0.points[0].location = (0.0, 1.7125)
    float_curve_0.points[0].handle_type = 'AUTO'
    float_curve_0.points[1].location = (1.0, 0.15)
    float_curve_0.points[1].handle_type = 'AUTO'
    #update curve after changes			
    float_curve.mapping.update()
    #Factor
    float_curve.inputs[0].default_value = 1.0
    #Value
    float_curve.inputs[1].default_value = 1.0
    float_curve.mapping.update()

    #node Spline Parameter.001
    spline_parameter_001 = log.nodes.new("GeometryNodeSplineParameter")
    spline_parameter_001.location = (-237.36212158203125, -283.28106689453125)
    spline_parameter_001.width, spline_parameter_001.height = 140.0, 100.0

    #node Subdivide Mesh
    subdivide_mesh = log.nodes.new("GeometryNodeSubdivideMesh")
    subdivide_mesh.location = (1586.4185791015625, 101.92158508300781)
    subdivide_mesh.width, subdivide_mesh.height = 140.0, 100.0
    #Level
    subdivide_mesh.inputs[1].default_value = 2

    #node Curve to Mesh
    curve_to_mesh = log.nodes.new("GeometryNodeCurveToMesh")
    curve_to_mesh.location = (728.54931640625, -397.77734375)
    curve_to_mesh.width, curve_to_mesh.height = 140.0, 100.0
    #Fill Caps
    curve_to_mesh.inputs[2].default_value = False

    #node Value
    value = log.nodes.new("ShaderNodeValue")
    value.location = (-258.5312805175781, -742.45166015625)
    value.width, value.height = 140.0, 100.0
    value.outputs[0].default_value = 0.015

    #node Math
    math = log.nodes.new("ShaderNodeMath")
    math.location = (-31.293228149414062, -781.10986328125)
    math.width, math.height = 140.0, 100.0
    math.operation = 'MULTIPLY'
    #Value
    math.inputs[0].default_value = 0.5
    #Value_001
    math.inputs[1].default_value = -1.0
    #Value_002
    math.inputs[2].default_value = 0.5

    #node Combine XYZ.001
    combine_xyz_1 = log.nodes.new("ShaderNodeCombineXYZ")
    combine_xyz_1.location = (143.76560974121094, -775.3092041015625)
    combine_xyz_1.width, combine_xyz_1.height = 140.0, 100.0
    #X
    combine_xyz_1.inputs[0].default_value = 0.0
    #Y
    combine_xyz_1.inputs[1].default_value = 0.0
    #Z
    combine_xyz_1.inputs[2].default_value = 0.0

    #node Transform
    transform = log.nodes.new("GeometryNodeTransform")
    transform.location = (321.2632141113281, -700.1317749023438)
    transform.width, transform.height = 140.0, 100.0
    #Translation
    transform.inputs[1].default_value = (0.0, 0.0, 0.0)
    #Rotation
    transform.inputs[2].default_value = (0.0, 0.0, 0.0)
    #Scale
    transform.inputs[3].default_value = (1.0, 1.0, 1.0)

    #node Combine XYZ
    combine_xyz = log.nodes.new("ShaderNodeCombineXYZ")
    combine_xyz.location = (144.08267211914062, -573.7868041992188)
    combine_xyz.width, combine_xyz.height = 140.0, 100.0
    #X
    combine_xyz.inputs[0].default_value = 0.0
    #Y
    combine_xyz.inputs[1].default_value = 0.0
    #Z
    combine_xyz.inputs[2].default_value = 0.0

    #node Curve Circle
    curve_circle = log.nodes.new("GeometryNodeCurvePrimitiveCircle")
    curve_circle.location = (-255.49168395996094, -592.7389526367188)
    curve_circle.width, curve_circle.height = 140.0, 100.0
    curve_circle.mode = 'RADIUS'
    #Resolution
    curve_circle.inputs[0].default_value = 5
    #Point 1
    curve_circle.inputs[1].default_value = (-1.0, 0.0, 0.0)
    #Point 2
    curve_circle.inputs[2].default_value = (0.0, 1.0, 0.0)
    #Point 3
    curve_circle.inputs[3].default_value = (1.0, 0.0, 0.0)
    #Radius
    curve_circle.inputs[4].default_value = 0.02500000037252903

    #node Transform.001
    transform_1 = log.nodes.new("GeometryNodeTransform")
    transform_1.location = (320.3553771972656, -462.5216979980469)
    transform_1.width, transform_1.height = 140.0, 100.0
    #Translation
    transform_1.inputs[1].default_value = (0.0, 0.0, 0.0)
    #Rotation
    transform_1.inputs[2].default_value = (0.0, 0.0, 0.0)
    #Scale
    transform_1.inputs[3].default_value = (1.0, 1.0, 1.0)

    #node Join Geometry
    join_geometry = log.nodes.new("GeometryNodeJoinGeometry")
    join_geometry.location = (532.006103515625, -457.07342529296875)
    join_geometry.width, join_geometry.height = 140.0, 100.0

    #initialize log links
    #subdivide_mesh.Mesh -> group_output.Geometry
    log.links.new(subdivide_mesh.outputs[0], group_output.inputs[0])
    #set_curve_radius.Curve -> curve_to_mesh.Curve
    log.links.new(set_curve_radius.outputs[0], curve_to_mesh.inputs[0])
    #join_geometry.Geometry -> curve_to_mesh.Profile Curve
    log.links.new(join_geometry.outputs[0], curve_to_mesh.inputs[1])
    #transform_001.Geometry -> join_geometry.Geometry
    log.links.new(transform_1.outputs[0], join_geometry.inputs[0])
    #curve_circle.Curve -> transform.Geometry
    log.links.new(curve_circle.outputs[0], transform.inputs[0])
    #transform.Geometry -> join_geometry.Geometry
    log.links.new(transform.outputs[0], join_geometry.inputs[0])
    #curve_circle.Curve -> transform_001.Geometry
    log.links.new(curve_circle.outputs[0], transform_1.inputs[0])
    #combine_xyz.Vector -> transform_001.Translation
    log.links.new(combine_xyz.outputs[0], transform_1.inputs[1])
    #combine_xyz_001.Vector -> transform.Translation
    log.links.new(combine_xyz_1.outputs[0], transform.inputs[1])
    #value.Value -> combine_xyz.X
    log.links.new(value.outputs[0], combine_xyz.inputs[0])
    #math.Value -> combine_xyz_001.X
    log.links.new(math.outputs[0], combine_xyz_1.inputs[0])
    #value.Value -> math.Value
    log.links.new(value.outputs[0], math.inputs[0])
    #math_001.Value -> set_curve_tilt.Tilt
    log.links.new(math_node_1.outputs[0], set_curve_tilt.inputs[2])
    #spline_parameter.Length -> math_001.Value
    log.links.new(spline_parameter.outputs[1], math_node_1.inputs[0])
    #curve_to_mesh.Mesh -> subdivide_mesh.Mesh
    log.links.new(curve_to_mesh.outputs[0], subdivide_mesh.inputs[0])
    #set_curve_tilt.Curve -> set_curve_radius.Curve
    log.links.new(set_curve_tilt.outputs[0], set_curve_radius.inputs[0])
    #float_curve.Value -> set_curve_radius.Radius
    log.links.new(float_curve.outputs[0], set_curve_radius.inputs[2])
    #spline_parameter_001.Factor -> float_curve.Value
    log.links.new(spline_parameter_001.outputs[0], float_curve.inputs[1])
    #group_input.Geometry -> set_curve_tilt.Curve
    log.links.new(group_input.outputs[0], set_curve_tilt.inputs[0])
#==============================================================


#==================== REGISTER / UNREGISTER ===================
def register():
    # START PANEL (OBJECT MODE)
    bpy.utils.register_class(AddonMainPanel)
    bpy.utils.register_class(BaseMeshesAdder)

def unregister():
    # START PANEL (OBJECT MODE)
    bpy.utils.unregister_class(AddonMainPanel)
    bpy.utils.unregister_class(BaseMeshesAdder)
#==============================================================




#==================== MAIN ====================================
if __name__ == "__main__":
    register()
#==============================================================
