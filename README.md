# architecture_misc
projet scolaire

Bonjour,

  Je n'ai pas eu le temps de rendre le proggramme fonctionnelle avec un exemple. cepandant tout le principe du code y est:
  -une partie permettant de lire un fichier texte comprenant un code assembleur
  -une partie simulant le fonctionnement d'une architecture misc
  
  j'ai choisie de faire une fonction thread pour chaque module suivant: instruction_fetch, instruction_decode, execute, memory_acces et write_back
  Elle se lanceront en simultané a chaque coup d'horloge. Les modules communiqueront a l'aide de (if_id, id_ex, etc ...) comme sur le shema .Seule les fonction
  decodes et write_back peuvent interagir avec le registre( On garde donc les deux port d'entré, donc d'ecriture et un port de sortie, de lecture).
  Les details sont expliquer dans les commentaire.
  
Les points à noter:

  -Les points à noterje comptais rajouter une fonction dans instruction_decode qui permetrai de mettre dans un tableau les addresse des valeurs qui serait decodé et qui aurai un decalage qui correspondrait au passage entre les modules, avec comme pour interrer de bloquer le module "fetch" et "decode" si une addresse est deja presentes dans ce tableau, en attendant que les autre instruction finisse leurs execution.
  
  
  -j'ai bien fais l'algorythme de bouth comme je l'avais mentionné en cours. je l'ai repris ici.
 
  
  
