# LABORATÓRIO 03

## GERAÇÃO DE CÓDIGO INTERMEDIÁRIO
## FONTE: MATERIAL DE LABORATÓRIO DE COMPILADORES DO PROFESSOR JUDSON SANTOS SANTIAGO

### O LINK DO GITHUB DO CÓDIGO FONTE DO ANALASIDOR LÉXICO PODE ACESSADO [AQUI](https://github.com/JudsonSS/Compiladores/tree/master/Labs/Lab11)


1. Modifique o projeto fornecido no Material de Apoio de forma a obter a geração de código intermediário para instruço es While. Ao rodar o programa em cima do exemplo Testes/teste4.cpp a saída deve ser obtida: 


teste4.cpp:
```
int main()
{
    int i; int j; float v; float x; float a[100]; bool flag;

    flag = true;
    while (flag)
    {
        do
        {
            i = i+1;
        } 
        while (a[i] < v);
        
        do
        {
            j = j-1;
        } 
        while (a[j] > v);
        
        if (i >= j)
            flag = false;
    
        x = a[i]; a[i] = a[j]; a[j] = x;
    }
}
```

saída:
```
        flag = true
L4:
        ifFalse flag goto L5
L1:
        t1 = i + 1
        i = t1
        t3 = a[i]
        t2 = t3 < v
        ifTrue t2 goto L1
L2:
        t4 = j - 1
        j = t4
        t6 = a[j]
        t5 = t6 > v
        ifTrue t5 goto L2
        t7 = i >= j
        ifFalse t7 goto L3
        flag = false
L3:
        t8 = a[i]
        x = t8
        t9 = a[j]
        a[i] = t9
        a[j] = x
        goto L4
L5:
```

<b>Dica:</b> Utilize as classes If e Do-While com modelo. Para alcançar esse resultado adicione co digo ao construtor e me todo Gen( ) da classe While.

A implementação deste laboratório vale 1 ponto.

O código fonte com o ajuste do novo requisito especificado deve ser disponibilizado preferencialmente através do link de um novo repositório no GitHub ou através de um arquivo compactado no formato .zip.

Os códigos disponibilizados para a atividade tem como autor o professor Judson Santiago e estão hospedados no GitHub sob a licença MIT. Isso significa que vocês têm permissão para baixar, modificar e reutilizar o código em seus próprios projetos, incluindo a criação de novos repositórios com funcionalidades adicionais. No entanto, é importante que, ao fazerem isso, vocês referenciem o autor original do código conforme exigido pela licença. Essa prática respeita os direitos do criador e promove uma cultura de colaboração e responsabilidade no uso de software livre.