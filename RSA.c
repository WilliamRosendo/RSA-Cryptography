#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int eh_primo(long long int a){
    long long int i;

    if(a == 2){
    	return 1;
    }else{
	    for(i=2; i<=sqrt(a); i++){
	        if(a % i == 0) {
	            return 0;
	        }
	    }
	}
	return 1;
}

int mdc(int a, int b){
    if(b==0){
    	return a;
    }else{
    	return mdc(b, a % b);
    }
}
int modulacao(int base, int exp, int mod){
	int resultado = 1, i;

  	for (i = 1; i <= exp; i++){
    	resultado = resultado * base;
    	resultado = resultado % mod;
  	}

    return resultado;
}

int chave_privada(long long int a, long long int b){
    long long int d, aux_penult, aux_ult, aux;
    long long int div[100]={0}, comb[100]={0,1};
    long long int auxd[100]={0}, i, resto, qnt = 0;
    
    aux = b;
    resto = b % a;

    while(resto != 0) {
        div[qnt] = b / a;
        b = a;
        a = resto;
        resto = b % a;
        qnt++;
    }
    for(i = 0; i < qnt; i++){
        auxd[i+2] = div[qnt-1-i];
    }
    for(i = 2; i < qnt + 2; i++){
        comb[i] = auxd[i] * comb[i-1] + comb[i-2];
    }
    
    aux_ult = comb[qnt+1];
    aux_penult = comb[qnt];

    if (qnt % 2 != 0){
        aux_ult = aux_ult * (-1);
    }else{
        aux_penult = aux_penult * (-1);
    }
    if (comb[qnt+1]>comb[qnt]){
        d = aux_ult;
    }else{
        d = aux_penult;
    }
    while(d<0){
    	d += aux;
    }
    return d;
}

void chave_publica(){
    long long int p, q, e, n, phi, opcao, i;
    printf("Digite um par de numeros primos p e q:\n");
    printf("Informe o valor de p: ");
    scanf("%lld", &p);
    while(!eh_primo(p)){
        printf("\np nao eh primo! \nDigite outro valor para p: ");
        scanf("%lld", &p);
    }
    printf("\n");
    printf("Informe o valor de q: ");
    scanf("%lld", &q);
    while(!eh_primo(q)){
        printf("\nq nao eh primo! \nDigite outro valor para q: ");
        scanf("%lld", &q);
    }
    printf("\n");

    n = p*q;
    phi = (p-1)*(q-1);

    while(n <= 26){
        printf("Primos muito pequenos! Digite outros valores para p e q:\n");
        printf("Informe o valor de p: ");
        scanf("%lld", &p);
        while(!eh_primo(p)){
            printf("p nao eh primo! Digite outro valor para p: ");
            scanf("%lld", &p);
        }
        printf("\n");
        printf("Informe o valor de q: ");
        scanf("%lld", &q);
        while(!eh_primo(q)){
            printf("q nao eh primo! Digite outro valor para q: ");
            scanf("%lld", &q);
        }
        printf("\n");
        n = p*q;
    }

    printf("Deseja ver as opcoes que voce tem para o valor de e?\n");
    printf("1 - SIM\n");
    printf("2 - NAO\n");
    scanf("%lld", &opcao);

    if(opcao == 1){
        printf("Voce pode escolher um dos numeros a seguir: ");
        for(i = 2; i < phi; i++){
            if(mdc(phi, i) == 1){
                printf("%lld ", i);
            }
        }
        printf("\n\n");
    }
    printf("Digite o valor de e: ");
    scanf("%lld", &e);

    while(e <= 1 || e >= phi || mdc(phi, e) != 1){
        printf("'e' eh <= a 1 ou eh >= a phi ou nao eh coprimo com phi!\n");
        printf("Tente outro valor para e: ");
        scanf("%lld", &e);
        printf("\n");
    }
    if(n>26 && e>1 && e<phi && mdc(phi, e)==1){
        FILE *chave;
        chave = fopen("chave_publica.txt", "w");
        fprintf(chave, "%lld %lld", n, e);
        fclose(chave);
        printf("\nChave publica criada!\n\n");
    }
}

void criptografia(){
    int qnt, i, n, e;
    int cifrados[1000], indices[1000];
    char texto[900];
    printf("Digite a mensagem de texto: ");
    getchar();
    gets(texto);
    for (i = 0; i < strlen(texto); ++i)
    {
        texto[i] = toupper(texto[i]);
    }
    printf("Digite a chave publica: ");
    scanf("%d%d", &n, &e);

    qnt = strlen(texto);
    
    for(i=0; i<qnt; i++){
        if(texto[i]== ' '){
            indices[i] = 26;
        }else{
            indices[i] = texto[i] - 'A';
        }
    }
    for(i=0;i<qnt;i++){
        cifrados[i] = modulacao(indices[i], e, n);
    }

    FILE *criptografada;
    criptografada = fopen("frase_criptografada.txt", "w");
    
    for(i=0; i<qnt; i++){
        fprintf(criptografada,"%d ",cifrados[i]);
    }
    fclose(criptografada);
    printf("\nFrase criptografada!\n\n");
}

void descriptografia(){
    long long int p, q, e, phi, n, d, qnt = 0, result[1000], i, texto[1000];
    char w;
    char alfabeto[27] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' '};
    printf("Digite p, q e e:\n");
    printf("Digite o valor de p: ");
    scanf("%lld", &p);
    printf("Digite o valor de q: ");
    scanf("%lld", &q);
    printf("Digite o valor de e: ");
    scanf("%lld", &e);
    printf("Digite a frase criptografada: ");

	while(w != '\n'){
    	scanf("%lld", &texto[qnt]);
    	w = getchar();
    	qnt++;
    }

    n = p*q;
    phi = (p-1)*(q-1);
    d = chave_privada(e, phi);

    for(i=0; i<qnt; i++){
    	result[i] = modulacao(texto[i], d, n);
    }

    FILE *descriptografada;
    descriptografada = fopen("frase_descriptografada.txt", "w");
    
    for(i=0; i<qnt; i++){
        fprintf(descriptografada,"%c",alfabeto[result[i]]);
    }
    fclose(descriptografada);
    printf("\nFrase descriptografada!\n\n");

}

int main(){
    int escolha;
    printf("Escolha uma opcao:\n");
    printf("1 - Gerar chave pública\n");
    printf("2 - Criptografar\n");
    printf("3 - Descriptografar\n");
    scanf("%d", &escolha);

    while(escolha != 1 && escolha != 2 && escolha != 3){
        printf("\nOpcao invalida! Escolha uma das opcoes abaixo:\n");
        printf("1 - Gerar chave pública\n");
        printf("2 - Criptografar\n");
        printf("3 - Descriptografar\n");
        getchar();
        scanf("%d", &escolha);
    }
    if(escolha==1){
        chave_publica();
    }
    if(escolha==2){
        criptografia();
    }
    if(escolha==3){
        descriptografia();
    }

    return 0;
}