import bpy

print("script en cours de fonctionnement")

obj = bpy.context.active_object

#test sur l'objet
if obj is None:
    print("ERREUR : Aucun objet sélectionné")
elif obj.type != 'MESH':
    print(f"ERREUR : L'objet sélectionné n'est pas un mesh ({obj.type})")
else:
    mesh = obj.data
    bpy.ops.object.mode_set(mode='OBJECT')

    print("float vertices[] = {")
    for v in mesh.vertices:
        x, y, z = v.co
        print(f"    {x:.6f}f, {y:.6f}f, {z:.6f}f,")
    print("};")
