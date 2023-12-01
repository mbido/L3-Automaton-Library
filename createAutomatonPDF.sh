
#!/bin/bash

# Vérifier si le répertoire "dotPrints" existe
if [ ! -d "dotPrints" ]; then
  echo "Le répertoire dotPrints n'existe pas."
  exit 1
fi

# Parcourir chaque fichier dans le répertoire
for file in dotPrints/*; do
  # Vérifier si le fichier est un fichier .dot
  if [[ $file == *.dot ]]; then
    # Utiliser la commande dot pour créer un fichier PDF basé sur le fichier .dot
    dot -Tpdf "$file" -o "${file%.dot}.pdf"
    rm "$file"
  fi
done

# Afficher un message de confirmation
echo "pdf created"
