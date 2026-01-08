import bpy

obj = bpy.context.active_object
mesh = obj.data

# Passe en mode objet pour éviter les bugs
bpy.ops.object.mode_set(mode='OBJECT')

print("float vertices[] = {")
for v in mesh.vertices:
    x, y, z = v.co
    print(f"    {x:.6f}f, {y:.6f}f, {z:.6f}f,")
print("};")
