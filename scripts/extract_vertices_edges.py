import bpy
import os

# Objet actif
obj = bpy.context.active_object
mesh = obj.data

# S'assurer que le mesh est triangulé
mesh.calc_loop_triangles()

# Chemin du fichier .blend
blend_path = bpy.data.filepath
folder = os.path.dirname(blend_path)

output_path = os.path.join(folder, "mesh_export.txt")

with open(output_path, "w") as f:

    # =====================
    # VERTICES
    # =====================
    f.write("float vertices[] = {\n")
    for v in mesh.vertices:
        f.write(f"    {v.co.x:.6f}f, {v.co.y:.6f}f, {v.co.z:.6f}f,\n")
    f.write("};\n\n")

    # =====================
    # INDICES (faces)
    # =====================
    f.write("unsigned int indices[] = {\n")
    for tri in mesh.loop_triangles:
        i0, i1, i2 = tri.vertices
        f.write(f"    {i0}, {i1}, {i2},\n")
    f.write("};\n\n")

    # =====================
    # INFO
    # =====================
    f.write(f"// Vertex count: {len(mesh.vertices)}\n")
    f.write(f"// Triangle count: {len(mesh.loop_triangles)}\n")

print(f"Export terminé : {output_path}")
