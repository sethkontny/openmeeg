#ifndef H_integrateur
#define H_integrateur

#include "vect3.h"
#include "triangle.h"
#include "mesh3.h"
#include "fcontainer.h"
#include <cmath>
#include <iostream>

inline void multadd (double &cible, const double multiplicateur, const double multiplie)
{
    cible+=multiplicateur*multiplie;
}

inline void multadd (Vect3 &cible, const double multiplicateur,  const Vect3 &multiplie)
{
    cible=cible+multiplicateur*multiplie;
}

// light class containing d Vect3
template <int d> class vect3array {
	Vect3 t[d];

public:
	vect3array() {};
	inline vect3array(double x) {
		for (int i=0;i<d;i++)
			t[i]=Vect3(x);
	}
	inline vect3array<d> operator*(double x) const {
		vect3array<d> r;
		for (int i=0;i<d;i++)
			r.t[i]=t[i]*x;
		return r;
	}
	inline Vect3 operator()(int i) const { return t[i]; }
	inline Vect3& operator()(int i) { return t[i]; }
};

template <int d>
inline void multadd (vect3array<d> &cible, const double multiplicateur,  const vect3array<d> &multiplie)
{
	for (int i=0;i<d;i++) {
		cible(i)=cible(i)+multiplicateur*multiplie(i);
	}
}


//#define OPTIMIZED_GAUSS //(Older version)
#define JK_GAUSS // (Most precise version)
// Coefficients d'integration tirés du livre de Marc Bonnet: Equations integrales..., Appendix B.3
// Quadrature rules are from Marc Bonnet's book: Equations integrales..., Appendix B.3


#ifdef JK_GAUSS

static const double cordBars[4][16][4]=  
{ 
        //parameters for N=3
    {
        {0.166666666666667,0.166666666666667,0.666666666666667,0.166666666666667}, 
        {0.166666666666667,0.666666666666667,0.166666666666667,0.166666666666667}, 
        {0.666666666666667,0.166666666666667,0.166666666666667,0.166666666666667},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0}
    }
    ,
        // parameters for N=6
    {
        {0.445948490915965,0.445948490915965,0.10810301816807,0.111690794839005},
        {0.445948490915965,0.10810301816807,0.445948490915965,0.111690794839005},
        {0.10810301816807,0.445948490915965,0.445948490915965,0.111690794839005},
        {0.091576213509771,0.091576213509771,0.81684757298045796,0.054975871827661},
        {0.091576213509771,0.81684757298045796,0.091576213509771,0.054975871827661},
        {0.816847572980458,0.091576213509771,0.091576213509771,0.054975871827661},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0}
    }
    ,
        // parameters for N=7
    {
        {0.333333333333333,0.333333333333333,0.333333333333333,0.1125},
        {0.470142064105115,0.470142064105115,0.059715871789770,0.066197076394253},
        {0.470142064105115,0.059715871789770,0.470142064105115,0.066197076394253},
        {0.059715871789770,0.470142064105115,0.470142064105115,0.066197076394253},
        {0.101286507323456,0.101286507323456,0.79742698535308798,0.062969590272414},
        {0.101286507323456,0.7974269853530880,0.101286507323456,0.062969590272414},
        {0.797426985353088,0.101286507323456,0.101286507323456,0.062969590272414},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0}
    }
    ,

        // parameters for N=16
    {
        {0.333333333333333,0.333333333333333,0.3333333333333333,0.072157803838893},
        {0.081414823414554,0.459292588292722,0.459292588292722,0.047545817133642},
        {0.459292588292722,0.081414823414554,0.459292588292722,0.047545817133642},
        {0.459292588292722,0.459292588292722,0.081414823414554,0.047545817133642},
        {0.898905543365937,0.050547228317031,0.050547228317031,0.016229248811599},
        {0.050547228317031,0.898905543365937,0.050547228317031,0.016229248811599},
        {0.050547228317031,0.050547228317031,0.898905543365937,0.016229248811599},
        {0.658861384496479,0.170569307751760,0.17056930775176099,0.051608685267359},
        {0.170569307751760,0.658861384496479,0.17056930775176099,0.051608685267359},
        {0.170569307751760,0.17056930775176099,0.658861384496479,0.051608685267359},
        {0.008394777409957,0.728492392955404,0.263112829634639,0.013615157087217},
        {0.728492392955404,0.008394777409957,0.263112829634639,0.013615157087217},
        {0.728492392955404,0.263112829634639,0.008394777409957,0.013615157087217},
        {0.008394777409957,0.263112829634639,0.728492392955404,0.013615157087217},
        {0.263112829634639,0.008394777409957,0.728492392955404,0.013615157087217},
        {0.263112829634639,0.728492392955404,0.008394777409957,0.013615157087217}
    }


}; // end of gaussTriangleParams

static const int nbPts[4]={3,6,7,16};

template<class T> class integrateur
{
private:
    // ordre numéro_du_noeud x_y_z_bary
    int ordre;

public:
    inline integrateur() {setOrdre(3);}
    inline integrateur(int ord) {setOrdre(ord);}
    inline ~integrateur() {}
    inline void setOrdre(int n)
    {
        if(n>=0 && n<4) ordre=n;
        else {std::cout<<"Unavalaible Gauss Order: "<<n<<std::endl; ordre = (n<1)?ordre=1:ordre;}
    }

    inline T integre ( const fContainer<T> &fc, const Triangle& Trg ,const Mesh& M) 
    {
        Vect3 sommets[3]={M.getPt(Trg.s1()),M.getPt(Trg.s2()),M.getPt(Trg.s3())};
	return triangle_integration(fc,sommets);
    }
    inline T  triangle_integration( const fContainer<T> &fc, Vect3 *vertices)
      {// compute double area of triangle defined by vertices
	Vect3 crossprod=(vertices[1]-vertices[0])^(vertices[2]-vertices[0]);
        double S= crossprod.norme();
        T result=0;
	static Vect3 zero(0.0,0.0,0.0);
	int i;
	for(i=0;i<nbPts[ordre];i++)
        {
            Vect3 v=zero;
            int j;
            for(j=0;j<3;j++) {
                v.multadd(cordBars[ordre][i][j],vertices[j]);
            }
            multadd(result,cordBars[ordre][i][3],fc.f(v));
        }
        return result*S;
      }
};

template<class T> class adaptive_integrator : public integrateur<T> {
 private:
  double tolerance;
 public:
  inline adaptive_integrator() {setTol(0.0001);}
  inline adaptive_integrator(double tol) {setTol(tol);}
  inline ~adaptive_integrator() {}
  inline void setTol(double tol)
    {
      tolerance = tol;
    }
    inline double norme(double a) {
   return fabs(a);
  }
  inline double norme(Vect3 a) {
    return a.norme();
  }
  inline T integrate ( const fContainer<T> &fc, const Triangle& Trg ,const Mesh& M) 
    { 
      int n=0;
      Vect3 sommets[3]={M.getPt(Trg.s1()),M.getPt(Trg.s2()),M.getPt(Trg.s3())};
      T I0=triangle_integration(fc,sommets);     
      return adaptive_integration(fc,sommets,I0,tolerance,n);
    }
  inline T  adaptive_integration(const fContainer<T> &fc,const Vect3 *vertices,T I0,const double tolerance,int n)
    { 
      Vect3 newpoint0(0.0,0.0,0.0);
      multadd(newpoint0,0.5,vertices[0]);
      multadd(newpoint0,0.5,vertices[1]);
      Vect3 newpoint1(0.0,0.0,0.0);
      multadd(newpoint1,0.5,vertices[1]);
      multadd(newpoint1,0.5,vertices[2]);
      Vect3 newpoint2(0.0,0.0,0.0);
      multadd(newpoint2,0.5,vertices[2]);
      multadd(newpoint2,0.5,vertices[0]);
      Vect3 vertices1[3]={vertices[0],newpoint0,newpoint2};
      Vect3 vertices2[3]={vertices[1],newpoint1,newpoint0};
      Vect3 vertices3[3]={vertices[2],newpoint2,newpoint1}; 
      Vect3 vertices4[3]={newpoint0,newpoint1,newpoint2}; 
      T I1=triangle_integration(fc,vertices1);
      T I2=triangle_integration(fc,vertices2);
      T I3=triangle_integration(fc,vertices3);
      T I4=triangle_integration(fc,vertices4);
      T somme=I1+I2+I3+I4;
      if (norme(I0-somme)>tolerance*norme(I0)){ 
	n=n+1;
	if (n<10) {
	  I1 = adaptive_integration(fc,vertices1,I1,tolerance,n);
	  I2 = adaptive_integration(fc,vertices2,I2,tolerance,n);
	  I3 = adaptive_integration(fc,vertices3,I3,tolerance,n);
	  I4 = adaptive_integration(fc,vertices4,I4,tolerance,n);	
	  I0 = I1+I2+I3+I4;
	}
	}
      //     	std::cout<<"  "<<n ;
	return I0;
      
      //  else {
      //			std::cout<<"depth: "<<n<<std::endl;
      //return I0;
      //}
    }
};


#else
#ifndef OPTIMIZED_GAUSS


// former version of integrator

// homogeneous coordinates of Gauss points
static const double cordBars[6][12][3]=
{
    {
        {1/3.,1/3.,1/3.},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0}
    }
    ,
    {
      {2/3.,1/6.,1/6.},
        {1/6.,2/3.,1/6.},
        {1/6.,1/6.,2/3.},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0}
    }
    ,
    {
        {1/3.,1/3.,1/3.},
        {3/5.,1/5.,1/5.},
        {1/5.,3/5.,1/5.},
        {1/5.,1/5.,3/5.},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0}
    }
    ,
    {
        {0.445948490915965,0.445948490915965,1-2*0.445948490915965},
        {0.445948490915965,1-2*0.445948490915965,0.445948490915965},
        {1-2*0.445948490915965,0.445948490915965,0.445948490915965},
        {0.091576213509771,0.091576213509771,1-2*0.091576213509771},
        {0.091576213509771,1-2*0.091576213509771,0.091576213509771},
        {1-2*0.091576213509771,0.091576213509771,0.091576213509771},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0}
    }
    ,
    {
        {1/3.,1/3.,1/3.},
        {(6-sqrt(15.))/21.,(6-sqrt(15.))/21.,1-2*(6-sqrt(15.))/21.},
        {(6-sqrt(15.))/21.,1-2*(6-sqrt(15.))/21.,(6-sqrt(15.))/21.},
        {1-2*(6-sqrt(15.))/21.,(6-sqrt(15.))/21.,(6-sqrt(15.))/21.},
        {(6+sqrt(15.))/21.,(6+sqrt(15.))/21.,1-2*(6+sqrt(15.))/21.},
        {(6+sqrt(15.))/21.,1-2*(6+sqrt(15.))/21.,(6+sqrt(15.))/21.},
        {1-2*(6+sqrt(15.))/21.,(6+sqrt(15.))/21.,(6+sqrt(15.))/21.},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0},
        {0.,0.,0}
    }
    ,
    {
        {0.063089011491502,0.063089011491502,1-2*0.063089011491502},
        {0.063089011491502,1-2*0.063089011491502,0.063089011491502},
        {1-2*0.063089011491502,0.063089011491502,0.063089011491502},
        {0.249286745170910,0.249286745170910,1-2*0.249286745170910},
        {0.249286745170910,1-2*0.249286745170910,0.249286745170910},
        {1-2*0.249286745170910,0.249286745170910,0.249286745170910},
        {0.310352451033785,0.053145049844816,1-0.310352451033785-0.053145049844816},
        {0.053145049844816,0.310352451033785,1-0.310352451033785-0.053145049844816},
        {0.310352451033785,1-0.310352451033785-0.053145049844816,0.053145049844816},
        {0.053145049844816,1-0.310352451033785-0.053145049844816,0.310352451033785},
        {1-0.310352451033785-0.053145049844816,0.310352451033785,0.053145049844816},
        {1-0.310352451033785-0.053145049844816,0.053145049844816,0.310352451033785}
    }
};

// weight of Gauss Points
static const double poids[6][12]=
{
    {1.0}
    ,
    {1/3.,1/3.,1/3.}
    ,
    {-9/16.,25/48.,25/48.,25/48.}
    ,
    {0.223381589678010,0.223381589678010,0.223381589678010,0.109951743655322,0.109951743655322,0.109951743655322}
    ,
    {9/40.,(155-sqrt(15.0))/1200.,(155-sqrt(15.0))/1200.,(155-sqrt(15.0))/1200.,(155+sqrt(15.0))/1200.,(155+sqrt(15.0))/1200.,(155+sqrt(15.0))/1200.}
    ,
    {0.050844906370206,0.050844906370206,0.050844906370206,0.116786275726378,0.116786275726378,0.116786275726378,0.082851075618374,0.082851075618374,0.082851075618374,0.082851075618374,0.082851075618374,0.082851075618374}
};

// Number of Gauss points
static const int nbPts[6]={1,3,4,6,7,12};

//Non optimized version of integrator
//    Homogeneous coordinates are used directly.
//    3 vectors are used instead of 2
//    this version can integrate a template valued funciton of R^3
template<class T> class integrateur
{
private:
    // ordre numéro_du_noeud x_y_z_bary
    double **pbarys;
    double *pds;
    int ordre;

public:
    inline integrateur() {setOrdre(1);}
    inline integrateur(int ord) {setOrdre(ord);}
    inline ~integrateur() {}
    inline void setOrdre(int n)
    {
        if(n>=0 && n<6) ordre=n;
        else {std::cout<<"Gauss order out of limits: "<<n<<std::endl; ordre = (n<1)?ordre=1:ordre;}
        pbarys=(double**)(cordBars[ordre]); pds=(double*)(poids[ordre]);
    }

    inline T integre ( const fContainer<T> &fc, const Triangle& Trg ,const Mesh& M) 
    {
        double S=Trg.getArea();
        T result=0;

        Vect3 sommets[3]={M.getPt(Trg.s1()),M.getPt(Trg.s2()),M.getPt(Trg.s3())};
        static Vect3 zero(0.0,0.0,0.0);

        for(int i=0;i<nbPts[ordre];i++) 
        {
            Vect3 v=zero;
            for(int j=0;j<3;j++) v.multadd(cordBars[ordre][i][j],sommets[j]);
            multadd(result,poids[ordre][i]*S,fc.f(v));
        }

        return result;
    }

};
#else

//! Some tables and constants for the Gauss quadrature  
static const int GaussQ2D_maxorder=12 ; // order is 0..11
static  const double 
GaussQ2D_nodes[GaussQ2D_maxorder][GaussQ2D_maxorder]={
    {0.},
    {-0.5773502691896257,0.5773502691896257},
    {-0.7745966692414834,0.,0.7745966692414834},
    {-0.861136311594053,-0.3399810435848562,0.3399810435848562,
    0.861136311594053},
    {-0.906179845938664,-0.5384693101056829,0.,0.5384693101056829,
    0.906179845938664},
    {-0.932469514203152, -0.6612093864662646, -0.2386191860831968,
    0.2386191860831968, 0.6612093864662646, 0.932469514203152},
    {-0.949107912342759, -0.7415311855993937, -0.4058451513773972,
    0., 0.4058451513773971, 0.7415311855993945, 0.949107912342759},
    {-0.960289856497537, -0.7966664774136262, -0.5255324099163289,
    -0.1834346424956498, 0.1834346424956498, 0.5255324099163289,
    0.7966664774136262, 0.960289856497537},
    {-0.968160239507626, -0.836031107326637, -0.6133714327005903,
    -0.3242534234038088, 0., 0.3242534234038088, 0.6133714327005908,
    0.836031107326635, 0.968160239507627},
    {-0.973906528517172,-0.865063366688984, -0.6794095682990246,
    -0.433395394129247, -0.1488743389816312, 0.1488743389816312,
    0.433395394129247, 0.6794095682990246, 0.865063366688984,
    0.973906528517172},
    {-0.97822865814604, -0.88706259976812, -0.7301520055740422,
    -0.5190961292068116, -0.2695431559523449, 0.,
    0.2695431559523449, 0.5190961292068117, 0.73015200557405,
    0.887062599768093, 0.978228658146058},
    {-0.981560634246732, -0.904117256370452, -0.7699026741943177,
    -0.5873179542866143, -0.3678314989981804, -0.1252334085114688,
    0.1252334085114688, 0.3678314989981804, 0.5873179542866143,
    0.7699026741943177,0.904117256370452, 0.981560634246732}} ;

    static const double GaussQ2D_weights[GaussQ2D_maxorder][GaussQ2D_maxorder]={
        {2.},{1., 1.},
        {0.5555555555555553, 0.888888888888889, 0.5555555555555553},
        {0.3478548451374539, 0.6521451548625462, 0.6521451548625462,
        0.3478548451374539},
        {0.2369268850561887, 0.4786286704993665, 0.5688888888888889,
        0.4786286704993665, 0.2369268850561887},
        {0.1713244923791709, 0.3607615730481379, 0.4679139345726913,
        0.4679139345726913, 0.3607615730481379, 0.1713244923791709},
        {0.129484966168868, 0.2797053914892783, 0.3818300505051186,
        0.4179591836734694, 0.3818300505051188, 0.279705391489276,
        0.1294849661688697},
        {0.1012285362903738, 0.2223810344533786, 0.3137066458778874,
        0.3626837833783619, 0.3626837833783619, 0.3137066458778874,
        0.2223810344533786, 0.1012285362903738},
        {0.0812743883615759, 0.1806481606948543, 0.2606106964029356,
        0.3123470770400029, 0.3302393550012597, 0.3123470770400025,
        0.2606106964029353, 0.1806481606948577, 0.0812743883615721},
        {0.06667134430868681, 0.149451349150573, 0.2190863625159832,
        0.2692667193099968, 0.2955242247147529, 0.2955242247147529,
        0.2692667193099968, 0.2190863625159832, 0.149451349150573,
        0.06667134430868681},
        {0.05566856711621584, 0.1255803694648743, 0.1862902109277404,
        0.2331937645919927, 0.2628045445102466, 0.2729250867779006,
        0.2628045445102466, 0.2331937645919933, 0.1862902109277339,
        0.1255803694649132, 0.05566856711616958},
        {0.04717533638647547, 0.1069393259953637, 0.1600783285433586,
        0.2031674267230672, 0.2334925365383534, 0.2491470458134027,
        0.2491470458134027, 0.2334925365383534, 0.2031674267230672, 
        0.1600783285433586, 0.1069393259953637, 0.04717533638647547}} ;

        template<class T> class integrateur
        {
        private:
            int ordre;

        public:
            integrateur() {setOrdre(1);}
            integrateur(int ord) {setOrdre(ord);}
            ~integrateur() {}

            void setOrdre(int n)
            {
                if(n>=0 && n<12) ordre=n;
                else {std::cout<<"Gauss order out of limits: "<<n<<std::endl; ordre = (n<1)?ordre=1:ordre;}
            }

            T integre ( const fContainer<T> &fc, const Triangle& Trg ,const Mesh& M)
            { 
                T sum=0 ;
                double u,v ;
                Vect3 t[3]={M.getPt(Trg.s1()),M.getPt(Trg.s2()),M.getPt(Trg.s3())};
                const Vect3 d10=t[1]-t[0] ;
                const Vect3 d20=t[2]-t[0] ;

                for (int iu=0;iu<ordre+1;iu++) { 
                    // sumv = integral wrt v for u fixed 
                    T sumv=0 ;
                    u=0.5*(GaussQ2D_nodes[ordre][iu]+1.0) ; 
                    for (int iv=0;iv<ordre+1;iv++) { 
                        Vect3 point ;
                        v=0.5*(GaussQ2D_nodes[ordre][iv]+1)*(1.0-u) ;
                        point=d10*u+d20*v+t[0] ;
                        multadd(sumv,GaussQ2D_weights[ordre][iv],fc.f(point));
                    } // for iv
                    multadd(sum,GaussQ2D_weights[ordre][iu]*0.5*(1.0-u),sumv) ;
                } // for iu
                return sum*Trg.getArea() ;
            } 
        };
#endif

#endif

#endif //JK_GAUSS

