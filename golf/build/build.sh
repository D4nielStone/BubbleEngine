#!/bin/bash

# Diretório do projeto
PROJECT_DIR="Golf"
# Binário do runtime
RUNTIME_BIN="golf"
# Nome do executável final
OUTPUT="jogo_final"

# Empacota os arquivos do projeto
cd $PROJECT_DIR
zip -r ../project_data.zip *
cd ..

# Anexa o ZIP ao binário do runtime
cat $RUNTIME_BIN project_data.zip > $OUTPUT

# Adiciona a assinatura e o tamanho dos dados ao final do executável
SIGNATURE="__PROJETO_EMBUTIDO__"
DATASIZE=$(wc -c < project_data.zip | xargs)
echo -n $SIGNATURE >> $OUTPUT
printf "%016X" $DATASIZE | xxd -r -p >> $OUTPUT

# Torna o executável finalizável
chmod +x $OUTPUT

# Limpa arquivos temporários
rm project_data.zip