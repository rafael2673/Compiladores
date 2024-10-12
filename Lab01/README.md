# LABORATÓRIO 01

## ANÁLISE LÉXICA
## FONTE: MATERIAL DE LABORATO RIO DE COMPILADORES DO PROFESSOR JUDSON SANTOS SANTIAGO

### O LINK DO GITHUB DO CÓDIGO FONTE DO ANALASIDOR LÉXICO PODE ACESSADO [AQUI](https://github.com/JudsonSS/Compiladores/tree/2e1b81ba859e18e938ea149d1cef2edea04dde36/Labs/Lab06)

1. Amplie o analisador léxico do material disponibilizado no link acima, para que ele
ignore comentários iniciando com barras duplas, até o final da linha, ou iniciando
com /* e finalizando com */.

As entradas abaixo devem ser válidas.

```
// uma expressão válida
total + 3 * 40
```

```
total + 3 * 40 // mais uma expressão válida
```

```
/* uma expressão válida */
total + 3 * 40 
```

```
total + 3 * 40 /* outra expressão válida */
```

```
total + // também
3 * 40  /* válido */
```

```
total + 3 /* também válido
* 40         mas ignora essa linha  */
```


A implementação deste laboratório vale 1 ponto. O código fonte com o ajuste do novo requisito especificado deve ser disponibilizado preferencialmente através do link de um novo repositório no GitHub ou através de um arquivo compactado no formato .zip.

Os códigos disponibilizados para a atividade tem como autor o professor Judson Santiago
e estão hospedados no GitHub sob a licença MIT. Isso significa que vocês têm permissão
para baixar, modificar e reutilizar o código em seus próprios projetos, incluindo a criação
de novos repositórios com funcionalidades adicionais. No entanto, é importante que, ao
fazerem isso, vocês referenciem o autor original do código conforme exigido pela licença.
Essa prática respeita os direitos do criador e promove uma cultura de colaboração e
responsabilidade no uso de software livre.
