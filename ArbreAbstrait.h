#ifndef ARBREABSTRAIT_H
#define ARBREABSTRAIT_H

// Contient toutes les déclarations de classes nécessaires
//  pour représenter l'arbre abstrait

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"
#include "Exceptions.h"
//#include "SymboleValue.h"

////////////////////////////////////////////////////////////////////////////////
class Noeud {
// Classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
// Remarque : la classe ne contient aucun constructeur
  public:
    virtual int  executer() =0 ; // Méthode pure (non implémentée) qui rend la classe abstraite
    virtual void ajoute(Noeud* instruction) { throw OperationInterditeException(); }
    virtual ~Noeud() {} // Présence d'un destructeur virtuel conseillée dans les classes abstraites
    virtual void traduireEnCPP(ostream& cout, unsigned int indentation) const =0;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud {
// Classe pour représenter un noeud "sequence d'instruction"
//  qui a autant de fils que d'instructions dans la séquence
  public:
     NoeudSeqInst();   // Construit une séquence d'instruction vide
    ~NoeudSeqInst() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();    // Exécute chaque instruction de la séquence
    void ajoute(Noeud* instruction);  // Ajoute une instruction à la séquence
    void traduireEnCPP(ostream& cout, unsigned int indentation) const;

  private:
    vector<Noeud *> m_instructions; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud {
// Classe pour représenter un noeud "affectation"
//  composé de 2 fils : la variable et l'expression qu'on lui affecte
  public:
     NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
    ~NoeudAffectation() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();        // Exécute (évalue) l'expression et affecte sa valeur à la variable
    void traduireEnCPP(ostream& cout, unsigned int indentation) const;

  private:
    Noeud* m_variable;
    Noeud* m_expression;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud {
// Classe pour représenter un noeud "opération binaire" composé d'un opérateur
//  et de 2 fils : l'opérande gauche et l'opérande droit
  public:
    NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
    // Construit une opération binaire : operandeGauche operateur OperandeDroit
   ~NoeudOperateurBinaire() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();            // Exécute (évalue) l'opération binaire)
    void traduireEnCPP(ostream& cout, unsigned int indentation) const;

  private:
    Symbole m_operateur;
    Noeud*  m_operandeGauche;
    Noeud*  m_operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////

class NoeudInstSinonsi : public Noeud {
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
  public:
    NoeudInstSinonsi(Noeud* condition, Noeud* sequence);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
    Noeud* getCondition() const;
    Noeud* getSequence() const;
    int executer() {return 0;};
   ~NoeudInstSinonsi() {} // A cause du destructeur virtuel de la classe Noeud
    //int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence
    void traduireEnCPP(ostream& cout, unsigned int indentation) const;

  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

class NoeudInstSinon : public Noeud {
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
  public:
    NoeudInstSinon(Noeud* sequence);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
    Noeud* getSequence() const;
    int executer() {return 0;};
   ~NoeudInstSinon() {} // A cause du destructeur virtuel de la classe Noeud
    //int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence
    void traduireEnCPP(ostream& cout, unsigned int indentation) const;

  private:
    Noeud*  m_sequence;
};

class NoeudInstSi : public Noeud {
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
  public:
    NoeudInstSi(Noeud* condition, Noeud* sequence, vector<NoeudInstSinonsi*> v);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
   ~NoeudInstSi() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence
    void ajouterSinonsi(NoeudInstSinonsi* sinonsi);
    void setSinon(NoeudInstSinon* sinon);
    void traduireEnCPP(ostream& cout, unsigned int indentation) const;

  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
    vector<NoeudInstSinonsi*> m_sinonsi;
    NoeudInstSinon* m_sinon;
};


class NoeudTantQue : public Noeud {
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
  public:
    NoeudTantQue(Noeud* condition, Noeud* sequence);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
   ~NoeudTantQue() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence
    void traduireEnCPP(ostream& cout, unsigned int indentation) const;

  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
};


class NoeudRepeter : public Noeud {
// Classe pour représenter un noeud "instruction repeter"
//  et ses 2 fils : la sequence du repeter et l'expression associée
  public:
    NoeudRepeter(Noeud* sequence, Noeud* condition);
     // Construit une "instruction repeter" avec sa condition et sa séquence d'instruction
   ~NoeudRepeter() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence
    void traduireEnCPP(ostream & cout,unsigned int indentation) const;
    
    
  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

///////////////////////////////////////////////////////////////////////////////////////

class NoeudPour : public Noeud {
// Classe pour représenter un noeud "instruction pour"
//  et ses 2 fils : la sequence du repeter et l'expression associée
  public:
    NoeudPour(Noeud* affectation, Noeud* expression, Noeud* affect, Noeud* sequence);
     // Construit une "instruction pour" avec sa condition et sa séquence d'instruction
   ~NoeudPour() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence
    void traduireEnCPP(ostream& cout, unsigned int indentation) const;

  private:
    Noeud*  m_affectation;
    Noeud*  m_expression;
    Noeud*  m_incrementation;
    Noeud*  m_sequence;
};

class NoeudLire : public Noeud {
    
    public:
        NoeudLire(vector<Noeud *>m_variable);
       ~NoeudLire() {}
        int executer();
        void traduireEnCPP(ostream& cout, unsigned int indentation) const;
       
       
    private:
       vector<Noeud *>m_variables; 
};

class NoeudEcrire : public Noeud {
    
    public:
        NoeudEcrire(vector<Noeud *>m_variable);
       ~NoeudEcrire() {}
        int executer();
        void traduireEnCPP(ostream& cout, unsigned int indentation) const;
       
       
    private:
       vector<Noeud *>m_variables; 
};


//class NoeudEcrire : public Noeud

#endif /* ARBREABSTRAIT_H */
