#include <iostream>
#include "PGM.hpp"
#include <sstream>
#include <cmath>
#include <string>
#include <cctype>
using namespace std;

#include <Eigen/Dense>
using namespace Eigen;

#define _PI	3.14159265358979323846

float degree2rad(float ang) {

	float r = ang * (float)(_PI / 180.0);
	return r;
}

Matrix3f getRotacao(float ang)
{
	Matrix3f M = Matrix3f::Identity();
	float angRad = degree2rad(ang); 
	float c = cos(angRad);
	float s = sin(angRad);

	M(0,0) = c;
	M(0,1) = -s;
	M(1,0) = s;
	M(1,1) = c;

	return M;
}


Matrix3f getTranslacao(float Tx, float Ty)
{
	Matrix3f M = Matrix3f::Identity();
	M(0,2) = Tx;
	M(1,2) = Ty;

	return M;
}

Matrix3f getEscala(float Sx, float Sy)
{
	Matrix3f M = Matrix3f::Identity();
	M(0,0) = Sx;
	M(1,1) = Sy;

	return M;
}

Matrix3f getCisalhamentoHorizontal(float Cx)
{
	Matrix3f M = Matrix3f::Identity();
	M(0,1) = Cx;

	return M;
}

Matrix3f getCisalhamentoVertical(float Cy)
{
	Matrix3f M = Matrix3f::Identity();
	M(1,0) = Cy;

	return M;
}

Matrix3f getReflexaoEixoX()
{
	Matrix3f M = Matrix3f::Identity();
	M(1,1) = -1;

	return M;
}

Matrix3f getReflexaoEixoY()
{
	Matrix3f M = Matrix3f::Identity();
	M(0,0) = -1;

	return M;
}

void Transf2DInv (PGM *imgE, PGM *imgS, Matrix3f Minv) 
{
	for (size_t y = 0; y < imgS->alt; y++)
	{
		for (size_t x = 0; x < imgS->larg; x++)
		{
			Vector3f coordS(x, y, 1);
			Vector3f coordE = Minv * coordS;

			coordE(0) = round(coordE(0));
			coordE(1) = round(coordE(1));

			if(coordValida(imgS, coordE.x(), coordE.y())) 
			{
				unsigned char c = getPixel(imgE, coordE(0), coordE(1));
				setPixel(imgS, coordS(0), coordS(1), c);
			}
		}
		
	}
}

int main (void) 
{
    char t;
    char e;
    Matrix3f Minv = Matrix3f::Identity();
    Matrix3f M = Matrix3f::Identity();

    PGM imgE, imgS;
    ler(&imgE, "numeros.pgm");
    criar(&imgS, imgE.larg, imgE.alt);

    float Tx, Ty;
    Tx = (imgE.larg - 1)/2.0;
    Ty = (imgE.alt - 1)/2.0;
    Matrix3f T = getTranslacao(Tx, Ty);
    Matrix3f Tinv = getTranslacao(-Tx, -Ty);

    do {
        system("cls");
        cout << "\n * Bem Vindo ao Programa, informe uma opcao do menu: *\n";
        cout << "\n * 1 - Aplicar uma transformacao a imagem            *\n";
        cout << "\n * 0 - Sair do programa                              *\n";
        cout << "\n * Sua escolha:                                      *\n";
        cin >> e;
        fflush(stdin);

        switch (e)
        {
            case '1':
                system("cls");
                cout << "\n * Bem Vindo ao Programa, informe uma opcao do menu: *\n";
                cout << "\n * T - Translacao                                    *\n";
                cout << "\n * R - Rotacao                                       *\n";
                cout << "\n * E - Escala                                        *\n";
                cout << "\n * H - Cisalhamento Horizontal                       *\n";
                cout << "\n * V - Cisalhamento Vertical                         *\n";
                cout << "\n * Y - Reflexao eixo Y                               *\n";
                cout << "\n * X - Reflexao eixo X                               *\n";
                cout << "\n * Sua escolha:                                      *\n";
                cin >> t;
                fflush(stdin);
                
                switch ((char) toupper(t))
                {
                    case 'T':  
                    {
                        system("cls");
                        float tx, ty;

                        cout << "\n Informe Tx: \n";
                        cin >> tx;
                        cout << "\n Informe Ty: \n";
                        cin >> ty;

                        Matrix3f T1 = getTranslacao(tx, ty);
                        Minv = T1 * Minv;
                        M = T * Minv * Tinv;
                        Transf2DInv(&imgE, &imgS, M.inverse());
                        gravar(&imgS, "saida.pgm");
                        cout << "\n Imagem gravada com sucesso! Retornando ao menu \n";
                        cout << "\n Aperte ENTER para continuar";
                        fflush(stdin);
                        getchar();
                        break;
                    }
                    case 'R':
                    {
                        system("cls");
                        PGM imgS2;
                        criar(&imgS2, imgE.larg, imgE.alt);
                        float ang;

                        cout << "\n Informe o angulo de rotacao (0 a 360): \n" << endl;
                        cin >> ang;

                        Matrix3f R = getRotacao(ang);
                        Minv = R * Minv;
                        M = T * Minv * Tinv;
                        Transf2DInv(&imgE, &imgS2, M.inverse());
                        gravar(&imgS2, "saida.pgm");
                        cout << "\n Imagem gravada com sucesso! Retornando ao menu \n";
                        cout << "\n Aperte ENTER para continuar";
                        fflush(stdin);
                        getchar();
                        break;
                    }
                    case 'E':
                    {
                        system("cls");
                        PGM imgS3;
                        criar(&imgS3, imgE.larg, imgE.alt);
                        float Sx, Sy;

                        cout << "\n Informe Sx: \n";
                        cin >> Sx;
                        cout << "\n Informe Sy: \n";
                        cin >> Sy;

                        Matrix3f E = getEscala(Sx, Sy);
                        Minv = E * Minv;
                        M = T * Minv * Tinv;
                        Transf2DInv(&imgE, &imgS3, M.inverse());
                        gravar(&imgS3, "saida.pgm");
                        cout << "\n Imagem gravada com sucesso! Retornando ao menu \n";
                        cout << "\n Aperte ENTER para continuar";
                        fflush(stdin);
                        getchar();
                        break;
                    }
                    case 'H':
                    {
                        system("cls");
                        PGM imgS4;
                        criar(&imgS4, imgE.larg, imgE.alt);
                        float Cx;
                        cout << "\n Digite o valor de Cx: \n";
                        cin >> Cx;

                        Matrix3f CX = getCisalhamentoHorizontal(Cx);
                        Minv = CX * Minv;
                        M = T * Minv * Tinv;
                        Transf2DInv(&imgE, &imgS4, M.inverse());
                        gravar(&imgS4, "saida.pgm");
                        cout << "\n Imagem gravada com sucesso! Retornando ao menu \n";
                        cout << "\n Aperte ENTER para continuar";
                        fflush(stdin);
                        getchar();
                        break;
                    }
                    case 'V':
                    {
                        system("cls");
                        PGM imgS5;
                        criar(&imgS5, imgE.larg, imgE.alt);
                        float Cy;
                        cout << "\n Digite o valor de Cy: \n";
                        cin >> Cy;

                        Matrix3f CY = getCisalhamentoVertical(Cy);
                        Minv = CY * Minv;
                        M = T * Minv * Tinv;
                        Transf2DInv(&imgE, &imgS5, M.inverse());
                        gravar(&imgS5, "saida.pgm");
                        cout << "\n Imagem gravada com sucesso! Retornando ao menu \n";
                        cout << "\n Aperte ENTER para continuar";
                        fflush(stdin);
                        getchar();
                        break;
                    }
                    case 'Y':
                    {
                        system("cls");
                        PGM imgS6;
                        criar(&imgS6, imgE.larg, imgE.alt);
                        Matrix3f Ry = getReflexaoEixoY();
                        Minv = Ry * Minv;
                        M = T * Minv * Tinv;
                        Transf2DInv(&imgE, &imgS6, M.inverse());
                        gravar(&imgS6, "saida.pgm");
                        cout << "\n Imagem gravada com sucesso! Retornando ao menu \n";
                        cout << "\n Aperte ENTER para continuar";
                        fflush(stdin);
                        getchar();
                        break;
                    }
                    case 'X':
                    {
                        system("cls");
                        PGM imgS7;
                        criar(&imgS7, imgE.larg, imgE.alt);
                        Matrix3f Rx = getReflexaoEixoX();
                        Minv = Rx * Minv;
                        M = T * Minv * Tinv;
                        Transf2DInv(&imgE, &imgS7, M.inverse());
                        gravar(&imgS7, "saida.pgm");
                        cout << "\n Imagem gravada com sucesso! Retornando ao menu \n";
                        cout << "\n Aperte ENTER para continuar";
                        fflush(stdin);
                        getchar();
                        break;
                    }
                    default:
                    {
                        cout << "\n Funcao nao encontrada \n";
                        break;
                    }
                }
                break;
            
            case '0':
            {
                system("cls");
                cout << "\n Saiu do Programa! \n";
                cout << "\n Pressione uma tecla para encerrar o programa.\n";
                getchar();
                return EXIT_SUCCESS; 
                break;
            }

            default:
                cout << "\n Funcao nao encontrada \n";
                break;
        }
    }while(t != 0);  
}