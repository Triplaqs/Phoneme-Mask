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
#file_path = "C:\\wsl.localhost\\Ubuntu\\home\\triplaqs\\Documents\\Code\\Phoneme-Mask\\src\\"
#file_path = "/home/triplaqs/Documents/Code/Phoneme-Mask/src/"
#file_path = r"\\wsl$\Ubuntu\home\triplaqs\Documents\Code\Phoneme-Mask\src\vertex.cpp"
file_path = "\\\\wsl$\\Ubuntu\\home\\triplaqs\\Documents\\Code\\Phoneme-Mask\\src\\"

#abs_path_os = os.path.abspath(folder)  #Pas bon, renvoie sur le dossier Blender
output_path = os.path.join(file_path, "vertex.cpp")
#print(f"Chemin absolu du dossier : {abs_path}")
#print(f"Chemin absolu du dossier (os): {abs_path_os}")

#pour récuperer mes shape keys
key_blocks = obj.data.shape_keys.key_blocks

with open(output_path, "w") as f:
    f.write("#include \"vertex.h\"\n\n")
    f.write("int n = 795;\n\n")
    f.write("Face facestruct;\n\n")
    f.write("Camera camera; \n\n\n")
    for key in key_blocks:
        # =====================
        # VERTICES
        # =====================
        f.write(f"const float {key.name}[] = ")
        f.write("{\n")
        for v in mesh.vertices:
            f.write(f"    {v.co.x:.6f}f, {v.co.y:.6f}f, {v.co.z:.6f}f,\n")
        f.write("};\n\n\n")

    # =====================
    # INDICES (faces)
    # =====================
    f.write("const unsigned int indices[] = {\n")
    for tri in mesh.loop_triangles:
        i0, i1, i2 = tri.vertices
        f.write(f"    {i0}, {i1}, {i2},\n")
    f.write("};\n\n\n")

    # =====================
    # FACE (phoneme affiché) (faces)
    # =====================
    f.write("std::vector<float> face;\n\n\n")

    # =====================
    # INFO
    # =====================
    f.write(f"// Vertex count: {len(mesh.vertices)}\n")
    f.write(f"// donc x3 = {3*len(mesh.vertices)} floats\n")
    f.write(f"// Triangle count: {len(mesh.loop_triangles)}\n")
    f.write(f"// donc x3 = {3*len(mesh.loop_triangles)} int\n\n\n")
    f.write("// Liste des phonèmes (en tant qu'états)\n")
    f.write("const float* phonemes[] = {\n neutre, phoneme_A\n};\n")

print(f"Export terminé : {output_path}")
