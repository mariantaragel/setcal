# Set Calculator

## Popis projektu

Cílem projektu je vytvořit program, který bude implementovat základní matematické operace nad množinami a binárními relacemi. Vstupem programu budou textová data reprezentující množiny a relace a zadání operací. Výsledek zpracování bude program tisknout na standardní výstup.

## Detailní specifikace

Vstupní data budou čtena ze souboru, jehož jméno bude zadáno argumentem příkazové řádky. Program provádí operace zadané v souboru po řádcích jedním průchodem. Výsledek zpracování každého řádku program tiskne na standardní výstup (tedy počet řádků na výstupu odpovídá počtu řádků vstupního souboru).

### Překlad zdrojového souboru

Program překládejte s následujícími argumenty:

```sh
$ gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal
```

### Syntax spuštění

Program se spouští v následující podobě (./setcal značí umístění a název programu):

```sh
./setcal FILE
``` 

### Formát vstupního souboru

 Textový soubor se skládá ze tří po sobě následujících částí:

<ol>
    <li>Definice univerza - jeden řádek začínající "U " a pokračující mezerou oddělenými prvky</li>
    <li>Definice množin a relací - jeden nebo více řádků začínající "S " nebo "R " a pokračující mezerou oddělenými prvky (řádek začínající "S" indikuje definici množiny, "R" slouží pro definici relace)</li>
    <li>Příkazy nad množinami a relacemi - jeden nebo více řádků začínající "C " a pokračující identifikátorem příkazu</li>
</ol>

### Univerzum

Prvky univerza jsou řetězce obsahující malá a velká písmena anglické abecedy. Délka řetězce je maximálně 30 znaků. Prvky univerza nesmí obsahovat identifikátory příkazů (viz níže) a klíčová slova true a false. Všechny prvky v množinách a relacích musí patřit do univerza. Příklad:

```sh
U Apple Lemon Orange Banana Peach
```

### Množiny

 Každá množina je definovaná na jednom řádku mezerou oddělenými prvky z univerza. Identifikátorem množiny je číslo řádku, na kterém je množina definovaná (vzhledem k tomu, že na prvním řádku souboru je univerzum, identifikátory množin tedy začínají číslem 2). Identifikátory množin jsou použity v operacích (viz níže). Příklad definice množiny:

```sh
S Apple Banana Peach
```

 ### Relace
 
 Každá relace je definována výčtem dvojic. Dvojice je ohraničená kulatými závorkami, první a druhý prvek dvojice jsou oddělené mezerou. Jednotlivé dvojice jsou oddělené mezerou. Příklad:
 
```sh
R (Apple Banana) (Apple Peach) (Apple Apple)
```

### Příkazy

Každý příkaz je definován na jednom řádku, začíná identifikátorem příkazu a argumenty příkazu jsou oddělené mezerou (od identifikátoru i mezi sebou). Argumenty příkazu jsou číselné identifikátory množin a relací (celá kladná čísla, číslo 1 identifikuje množinu univerza). Příklad:

```sh
C minus 1 2
```

### Příkazy nad množinami

Příkaz pracuje nad množinami a jeho výsledkem je buď množina (v tom případě tiskne množinu ve stejném formátu jako se očekává ve vstupním souboru, tj. začíná "S " a pokračuje mezerou oddělenými prvky) nebo je výsledkem pravdivostní hodnota (v tom případě tiskne true nebo false na samostatný řádek) nebo je výsledkem přirozené číslo (které se tiskne na samostatný řádek).

<ul>
    <li>empty A - tiskne true nebo false podle toho, jestli je množina definovaná na řádku A prázdná nebo neprázdná</li>
    <li>card A - tiskne počet prvků v množině A (definované na řádku A)</li>
    <li>complement A - tiskne doplněk množiny A</li>
    <li>union A B - tiskne sjednocení množin A a B</li>
    <li>intersect A B - tiskne průnik množin A a B</li>
    <li>minus A B - tiskne rozdíl množin A \ B</li>
    <li>subseteq A B - tiskne true nebo false podle toho, jestli je množina A podmnožinou množiny B</li>
    <li>subset A B - tiskne true nebo false, jestli je množina A vlastní podmnožina množiny B</li>
    <li>equals A B - tiskne true nebo false, jestli jsou množiny rovny</li>
</ul>

### Příkazy nad relacemi

Příkaz pracuje nad relacemi a jeho výsledkem je buď pravdivostní hodnota (tiskne true nebo false), nebo množina (tiskne množinu ve formátu jako ve vstupnímu souboru).

<ul>
    <li>reflexive R - tiskne true nebo false, jestli je relace reflexivní</li>
    <li>symmetric R - tiskne true nebo false, jestli je relace symetrická</li>
    <li>antisymmetric R - tiskne true nebo false, jestli je relace antisymetrická</li>
    <li>transitive R - tiskne true nebo false, jestli je relace tranzitivní</li>
    <li>function R - tiskne true nebo false, jestli je relace R funkcí</li>
    <li>domain R - tiskne definiční obor funkce R (lze aplikovat i na relace - první prvky dvojic)</li>
    <li>codomain R - tiskne obor hodnot funkce R (lze aplikovat i na relace - druhé prvky dvojic)</li>
    <li>injective R A B - tiskne true nebo false, jestli je funkce R injektivní, A a B jsou množiny; a∈A, b∈B, (a,b)∈R</li>
    <li>surjective R A B - tiskne true nebo false, jestli je funkce R surjektivní, A a B jsou množiny; a∈A, b∈B, (a,b)∈R</li>
    <li>bijective R A B - tiskne true nebo false, jestli je funkce R bijektivní. A a B jsou množiny; a∈A, b∈B, (a,b)∈R</li>
</ul>

### Implementační detaily

<ul>
    <li>Maximální podporovaný počet řádků je 1000</li>
    <li>Na pořadí prvků v množině a v relaci na výstupu nezáleží</li>
    <li>Všechny prvky množin a v relacích musí patřit do univerza. Pokud se prvek v množině nebo dvojice v relaci opakuje, jedná se o chybu</li>
</ul>

## Příklady použití

```sh
$ cat sets.txt
U a b c x y z
S a b c x
S x y z
C intersect 2 3
C minus 2 3
```

```sh
$ ./setcal sets.txt
U a b c x y z
S a b c x
S x y z
S x
S a b c
```

```
$ cat rel.txt
U dad mom girl boy man woman
R (dad boy) (dad girl) (mom boy) (mom girl)
R (dad man) (boy man) (mom woman) (girl woman)
C codomain 2
C function 3
```

```
$ ./setcal rel.txt
U dad mom girl boy man woman
R (dad boy) (dad girl) (mom boy) (mom girl)
R (dad man) (boy man) (mom woman) (girl woman)
S boy girl
true
```

## Authors

- [mariantaragel](https://github.com/mariantaragel)
- [xtroitgeo](https://github.com/xtroitgeo)
- [TProkop01](https://github.com/TProkop01)
