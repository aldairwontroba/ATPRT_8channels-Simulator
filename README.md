Passo a passo para instalar o ATPRT_8channes no Windows rodando como Simulador digital em Tempo Real:

	>>: Instale o ATP (InstATP124.exe) no caminho "C:\ATP"								


	>>: Instale o ATPDraw (ATPDraw60_Install.zip) no caminho "C:\ATP\ATPDraw"

	>>: Instale o ATP124_lib.exe
{
Nescessario para recriar o ATP com as funçoes adicionais;
Uma pasta ATPdata será criada em Documentos, esta pasta irá conter:
\ATPdata\projects // não será usada
\ATPdata\work     // onde ficaram os arquivos de trabalho do compilador gcc 2.5 do ATP
\ATPdata\mytpbig  // pasta onde irá criar o novo ATP e onde ficarão os arquivos de compilação
}
	>>: Extrair o arquivo pa_stable_v190600_20161030.tgz em qualquer lugar	(Desktop por exemplo)
{
Esta pasta irá conter todos as bibliotecas de audio utilizadas. 
Será necessario compilar toda essa pasta em outro gcc mais atual, para isso instale o minGW (proximo >>)
Links importantes: 
http://portaudio.com/docs/v19-doxydocs/tutorial_start.html
}
	>>: Copie a pasta asiosdk_2.3.3 para dentro da pasta do portaudio (onde foi extraido)

	>>: Instale o driver ASIO4ALL_2_14_English.exe (instalação sem misterios)

	>>: Instale o mingw-get-setup.exe
{
Esse compilador será usado para complilar e gerar as libs do portaudio e o SegundoPrograma
Na instalação, selecione as opções basicas.
Marcar as opcões de package após a instalação instalando as seleções
O mingw é basicamente o gcc para windows
}
	>>: Apos instalado o mingw, abra o cmd do windows (como admin) e digite:
{
	set PATH=c:\MinGW\bin;c:\MinGW\msys\1.0\bin;%PATH%
	bash
agora o gcc deve estar disponivel 
	gcc --version
agora mova o caminho do cmd para a pasta portaudio que foi extraida
use cd.. para voltar pastas e cd ./ para andar nas pastas
agora digite:
	./configure --with-winapi=asio --with-asiodir=./asiosdk_2.3.3
	makemake
isso irá compilar o portaudio e gerar as lib necessárias
deixe o pront aberto pq será necessario depois
}
	>>: Copie a pasta SegundoPrograma para a area de trabalho

	>>: Copie as pastas Lib e include que estão dentro do portaudio (apos ter finalizado o make) para SegundoPrograma

	>>: Copie os arquivos "fgnmod.f" e "teste_foreign_model.cpp" para a pasta "mytpbig" do ATPdata (que está em documentos)

	>>: Abra o arquivo "teste_foreign_model.cpp" e edite a linha  startup("C:\\Users\\aldair\\Desktop\\SegundoPrograma\\AudioTransmiterATP.exe"); trocando o caminho do arquivo para o correspondente.
{
o primeiro arquivo diz pro ATP que uma função com um nome especifico será chamada a cada passo de interação.
o segundo arquivo contem as funções que são chamadas a cada passo de interação do ATP;
}
	>>: Copie o arquivo "Makefile_FM" para a pasta "C:\ATP\atpmingw\make"
{
este arquivo contem as instruções para o confilador criar o atp
}
	>>: Abra esse arquivo "Makefile_FM" com bloco de notas e edite o caminho de MYDOCS, (apenas substitua o nome de usuario para o seu usuario)

MUINTO IMPORTANTE:
não substitua os arquivos nesta pasta a seguir, apenas cole e ignore os arquivos igual.
	>>: Na pasta gcc-2.95.2, copie lib e include que está em \i386-mingw32\. Cole em C:\ATP\atpmingw\make\MinGW mas não substituas os arquivos.

	>>: Copie a header stdafx.h para a pasta C:\ATP\atpmingw\make\MinGW\include
{
esta header esta faltando tanto no compilador do ATP quanto no novo MinGW
}
	>>: Copie a mesma stdafx.h para a pasta C:\MinGW\include mas agora abra ela e edite:
{
descomente a linha:
	//#include <oledlg.h> /* OPENFILENAME */ //esta header não funciona no compilador do ATP
apagando os //
esta header pode ter funcionalidade no futuro.
}
	>>: Abra o ATPlauncher (deve ser encontrado na pesquisa do windows) clique em "Tools" e "Make Tpbig.exe"

	>>: Certifique-se de que o caminho para "Makefile" está apontando para o arquivo "Makefile_FM" copiado anteriormente no caminho "C:\ATP\atpmingw\make"
{
clique em salvar apos ter mudado o caminho.
}
	>>: Clique em "Make"
{
aqui podem aparecer varios erros de compilação, devem ser corrigidos de alguma forma.
nessa parte será criado o novo ATP em mytpbig como "mytpbig.exe"
}
	>>: Agora no cmd (bash-3.1$) onde foi compilado o portaudio mova o caminho para a pasta SegundoPrograma e digite:
{
	make
agora deve ter sido compilado e gerado o executavel
}
	>>: Copie o arquivo "mytpbig.exe"que foi gerado no compilar o ATP  para a pasta "C:\ATP\atpmingw"

	>>: Encontre o arquivo STARTUP dentro da pasta "C:\ATP\atpmingw" encontre e modifique a opção NEWPL4 para o valor 2
{
para funcionar o plot no ATP
}
	>>: Copie o arquivo "runATP.bat" para a pasta "C:\ATP\Tools" 
{
esse arquivo rodo o mytpbig criado, existe 2 opções dentro deste arquivo:
é possivel rodar o mytpbig.exe com prioridade de execução maior, para isso abra o arquivo "runATP.bat" e leia ele, lá dentro tem as instruções. Basicamente deve-se comentar uma linha e descomentar outra.
No entanto essa opção não permite visualizar a janela de ERROS do ATP, então use ela apenas quando necessário.
Na realidade essa opção não faz tanta diferença.
}
	>>: copie de C:/MinGW/ as dll: libstdc++-6.dll libgcc_s_dw2-1.dll para a pasta: C:\Windows\SysWOW64

	>>: abra o arquivo TESTE_FM.acp e configure o ATP em "Tools" "Options" "Preferences" e certifique-se de que a secção ATP está o caminho "C:\ATP\tools\runATP.bat" final .bat

	>>: se ocorrer erro na hora de rodar a simulação dizendo que esta faltando uma dll, é só pesquisar ela dentro do MinGW e copiar para a pasta do windows, como feito acima.

	>>: agora é só adicionar o bloco do models já configurado e rodar. As duas entradas podem ser tensão ou corrente, basta configurar "Edit definitions" "Nodes" "Kind" 1=corrent 2=tensão, para mais opções consulte o help do ATP

	>>: as pastas portaudio e o compilador mingw instalado assim como as pastas work\Include e work\lib podem ser deletados do computador, eles não seram mais necessários.

#IMPORTANTE: 	quando executar as simulações, certifique-se de que não há nenhum efeito de audio (como aumento de grave) sendo aplicado pelo windows ou pelo gerenciador de audio, pois irá distorcer a forma de onda.

	>>: pra cada computador é necessario realizar uma calibração de amplitude e fase. Cada canal de audio sai com um poouco de diferença um do outro, essa diferença
pode ser compensada via software. Para realizar essa calibração é precio de um osciloscópio. para ajustar cada canal é necessario alterar estas variaveis no codigo do AudioTransmiterATP.cpp
cade vez de ser recompilado este .cpp

//Ajuste de amplitude para cada canal
#define amplitude1 0.8715 
#define amplitude2 0.878
#define amplitude3 0.9195
#define amplitude4 0.9187
#define amplitude5 0.9005
#define amplitude6 0.8928 
#define amplitude7 0.89777 
#define amplitude8 0.899

//Ajuste de fase para cada canal : o erro é <0.5 graus para 60Hz
#define atraso1 0.000231 //seconds
#define atraso2 0.000231 //seconds 
#define atraso3 0.000587 //seconds
#define atraso4 0.000587 //seconds
#define atraso5 0.000231 //seconds
#define atraso6 0.000231 //seconds
#define atraso7 0.0 //seconds
#define atraso8 0.0 //seconds
