echo off
:: -------------------------------------------
:: Arquivo responsavel por executar TPBIG.exe
:: Desenvolvido por Adriano Marchesan
:: 25 de Novembro de 2015
:: -------------------------------------------
:: Seta o diretorio do TPBIG.exe
SET GNUDIR=C:\ATP\atpmingw\
:: Executa o mytpbig.exe
%GNUDIR%mytpbig.exe BOTH %~nx1 s -R
PAUSE
:: Deleta arquivos auxiliares e temporarios
DEL dum*.bin
DEL *.tmp