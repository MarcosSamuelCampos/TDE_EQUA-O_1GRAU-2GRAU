#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <locale.h>

// essa parte de cima foi separado para o compilador não da erro na hora da leitura do codigo
// deixando a estrutura do codigo mais limpa 

typedef enum { GRAU_INVALIDO, PRIMEIRO_GRAU, SEGUNDO_GRAU } GrauEquacao;
void limparString(char *s);
int extrairCoeficiente(const char *eq, const char *pat, double *coef) ;
GrauEquacao detectarEquacaoString(const char *equacao);
void resolverPrimeiroGrau(double coeficienteA, double coeficienteB);
void resolverSegundoGrau(double coeficienteA, double coeficienteB, double coeficienteC) ;

int main() {
	setlocale(LC_ALL, "Portuguese");
    char linha[100];
    printf("Digite a equação: ");
    if (!fgets(linha, sizeof(linha), stdin)){
    	return 0;	
	} 
    // remove '\n' e espaços
    if (linha[strlen(linha)-1] == '\n') linha[strlen(linha)-1] = '\0';
    limparString(linha);
	// após limparString(linha);
char *igual = strchr(linha, '=');
if (igual) *igual = '\0';   // termina a string ali, removendo tudo que vem depois

    GrauEquacao grau = detectarEquacaoString(linha);
    if (grau == GRAU_INVALIDO) {
        printf("Equacao invalida! \n");
        return 0;
    }

    double coeficienteA =0, coeficienteB = 0, coeficienteC=0;
    
    if (grau == SEGUNDO_GRAU) {
        // extrai antes de "x^2"
        if (!extrairCoeficiente(linha, "x^2", &coeficienteA))coeficienteA = 0;
        // extrai b antes de "x" que não seja "x^2"
        // cria uma cópia onde "x^2" é apagado, para não confundir
        char tmp[256];
        strcpy(tmp, linha);
       char *p2 = strstr(tmp, "x^2");
if (p2) {
    // move o ponteiro até o início do coeficiente
    char *start = p2;
    while (start > tmp && (isdigit((unsigned char)start[-1])
           || start[-1]=='+' || start[-1]=='-' || start[-1]=='.'))
        start--;
    // remove coeficiente x^2 inteiro
    memmove(start, p2+3, strlen(p2+3)+1);
}
        if (!extrairCoeficiente(tmp, "x", &coeficienteB)) coeficienteB = 0;

        char *p;        
        // elimina termo linear
        p = strstr(tmp, "x");
        if (p) { // pula o coeficiente e o 'x'
            // para encontrar o início do coeficiente:
            char *start = p;
            while (start > tmp && (isdigit((unsigned char)start[-1])|| start[-1]=='-'|| start[-1]=='+'|| start[-1]=='.'))
                start--;
            // e move o resto
            memmove(start, p+1, strlen(p+1)+1);
        }
        if (strlen(tmp) > 0)
            coeficienteC = strtod(tmp, NULL);
        else
            coeficienteC = 0;
        printf("Equacao de 2o grau detectada: a=%.2f, b=%.2f, c=%.2f\n", coeficienteA, coeficienteB, coeficienteC);
        resolverSegundoGrau(coeficienteA,coeficienteB,coeficienteC);
    }
    else if (grau == PRIMEIRO_GRAU) {
        // extrai antes de "x"
        if (!extrairCoeficiente(linha, "x", &coeficienteA)) coeficienteA = 0;
        // retira o ax de uma cópia para isolar b
        char tmp[100];
        strcpy(tmp, linha);
        char *p = strstr(tmp, "x");
        if (p) {
            char *start = p;
            while (start > tmp && (isdigit((unsigned char)start[-1])|| start[-1]=='-'|| start[-1]=='+'|| start[-1]=='.'))
                start--;
            memmove(start, p+1, strlen(p+1)+1);
        }
        if (strlen(tmp)>0)
            coeficienteB = strtod(tmp, NULL);
        else
            coeficienteB = 0;
        printf("Equacao de 1o grau detectada: a=%.2f, b=%.2f\n", coeficienteA, coeficienteB);
        resolverPrimeiroGrau(coeficienteA,coeficienteB);
    }

    return 0;
}

//essa parte fica as funções completas  
void limparString(char *s) {
    // remove espaços e tabs
    char *charr = s, *src = s;
    while (*src) {
        if (!isspace((unsigned char)*src)) *charr++ = *src;
        src++;
    }
    *charr = '\0';
}

int extrairCoeficiente(const char *eq, const char *pat, double *coef) {
    char *pos = strstr(eq, pat);
    if (!pos) return 0;
    // separa substrings
    size_t len = pos - eq;
    if (len == 0) {
        // se não tem número entao coeficiente ±1
        *coef = 1.0;
    } else if (len == 1 && eq[0] == '-') {
        *coef = -1.0;
    } else {
        char buf[64];
        strncpy(buf, eq, len);
        buf[len] = '\0';
        *coef = strtod(buf, NULL);
    }
    return 1;
}

void resolverPrimeiroGrau(double coeficienteA, double coeficienteB) {
    if (coeficienteA == 0) {
        if (coeficienteB == 0) {
        	printf("Infinitas solucoes.\n");
		}
        else{
        	printf("Sem solucao.\n");
		}        
    } else {
        double x = -coeficienteB / coeficienteA;
        printf("Possui soluçao real: sim | solucao: x = %.2f\n", x);
    }
}

GrauEquacao detectarEquacaoString(const char *equacao) {
    if (strstr(equacao, "x^2")) return SEGUNDO_GRAU;
    if (strstr(equacao, "x"))   return PRIMEIRO_GRAU;
    return GRAU_INVALIDO;
}

void resolverSegundoGrau(double coeficienteA, double coeficienteB, double coeficienteC) {
    double delta = (coeficienteB*coeficienteB) - 4*coeficienteA*coeficienteC;
    if (delta < 0) {
        printf("Possui solução real: Nao | solução: N.A\n");
    } else if (delta == 0) {
        double x = -coeficienteB/(2*coeficienteA);
        printf("Possui solução real: Sim | solução: x = %.2f\n", x);
    } else {
        double sqrtD = sqrt(delta);
        double x1 = (-coeficienteB + sqrtD)/(2*coeficienteA);
        double x2 = (-coeficienteB - sqrtD)/(2*coeficienteA);
        printf("Possui solução real: Sim | solução: x = %.2f, x = %.2f\n", x2, x1);
    }
}
