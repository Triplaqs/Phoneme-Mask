import bpy
import os

# ---- Récupération de l'objet sélectionné ----
obj = bpy.context.active_object

if obj is None:
    raise RuntimeError("Aucun objet sélectionné")
if obj.type != 'MESH':
    raise RuntimeError("L'objet sélectionné n'est pas un mesh")

mesh = obj.data
bpy.ops.object.mode_set(mode='OBJECT')

# ---- Détermination du dossier de sortie ----
# 1) dossier du script Python (si sauvegardé)
script_path = bpy.data.filepath
script_dir = os.path.dirname(script_path)

# fallback si le .blend n'est pas sauvegardé
if not script_dir:
    script_dir = os.path.expanduser("~")

output_path = os.path.join(script_dir, "vertices.txt")

# ---- Écriture du fichier ----
with open(output_path, "w", encoding="utf-8") as f:
    f.write("float vertices[] = {\n")
    for v in mesh.vertices:
        x, y, z = v.co
        f.write(f"    {x:.6f}f, {y:.6f}f, {z:.6f}f,\n")
    f.write("};\n")

print(f"Vertices exportés dans : {output_path}")
