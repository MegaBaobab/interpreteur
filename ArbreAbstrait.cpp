#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"
#include <typeinfo>

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
  for (unsigned int i = 0; i < m_instructions.size(); i++)
    m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
  int valeur = m_expression->executer(); // On exécute (évalue) l'expression
  ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
  int og, od, valeur;
  if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
  if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
  // Et on combine les deux opérandes en fonctions de l'opérateur
  if (this->m_operateur == "+") valeur = (og + od);
  else if (this->m_operateur == "-") valeur = (og - od);
  else if (this->m_operateur == "*") valeur = (og * od);
  else if (this->m_operateur == "==") valeur = (og == od);
  else if (this->m_operateur == "!=") valeur = (og != od);
  else if (this->m_operateur == "<") valeur = (og < od);
  else if (this->m_operateur == ">") valeur = (og > od);
  else if (this->m_operateur == "<=") valeur = (og <= od);
  else if (this->m_operateur == ">=") valeur = (og >= od);
  else if (this->m_operateur == "et") valeur = (og && od);
  else if (this->m_operateur == "ou") valeur = (og || od);
  else if (this->m_operateur == "non") valeur = (!og);
  else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
  }
  return valeur; // On retourne la valeur calculée
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence, vector<NoeudInstSinonsi*> v)
: m_condition(condition), m_sequence(sequence), m_sinon(nullptr) {
    for(auto &sinonsi : v){
        m_sinonsi.push_back(sinonsi);
    }
}

int NoeudInstSi::executer() {
  int i = 0, b=0;
  if (m_condition->executer()){
    m_sequence->executer();
  }else {
      while (b==0 && i<m_sinonsi.size()){
          if (m_sinonsi[i]->getCondition()->executer()) {
              m_sinonsi[i]->getSequence()->executer();
              b=1;
          }else{
              i++;
          }
      }
      if (b==0 && m_sinon != nullptr){
          m_sinon->getSequence()->executer();
          
      }
  }
 
  return 0; // La valeur renvoyée ne représente rien !
}


void NoeudInstSi::ajouterSinonsi(NoeudInstSinonsi* sinonsi) {
    m_sinonsi.push_back(sinonsi);
}


void NoeudInstSi::setSinon(NoeudInstSinon* sinon) {
    m_sinon=sinon;
}


////////////////////////////////////////////////////////////////////////////////
// NoeudInstSinon
////////////////////////////////////////////////////////////////////////////////

NoeudInstSinon::NoeudInstSinon(Noeud* sequence)
:m_sequence(sequence){
}

Noeud* NoeudInstSinon::getSequence() const {
    return m_sequence;
}



////////////////////////////////////////////////////////////////////////////////
// NoeudInstSinonsi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSinonsi::NoeudInstSinonsi(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence){
}

Noeud* NoeudInstSinonsi::getCondition() const {
    return m_condition;
}

Noeud* NoeudInstSinonsi::getSequence() const {
    return m_sequence;
}

////////////////////////////////////////////////////////////////////////////////
// NoeudTantQue
////////////////////////////////////////////////////////////////////////////////

NoeudTantQue::NoeudTantQue(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudTantQue::executer() {
  while (m_condition->executer()) m_sequence->executer();
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudRepeter
////////////////////////////////////////////////////////////////////////////////

NoeudRepeter::NoeudRepeter(Noeud* sequence, Noeud* condition) : 
 m_sequence(sequence),m_condition(condition) {
}

int NoeudRepeter::executer() {
    do {
        m_sequence->executer();
    }
    while (!m_condition->executer());
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudPour
////////////////////////////////////////////////////////////////////////////////

NoeudPour::NoeudPour(Noeud* affectation, Noeud* expression, Noeud* affecta, Noeud* sequence) : 
 m_affectation(affectation),m_expression(expression),m_incrementation(affecta), m_sequence(sequence) {
}

int NoeudPour::executer() {
    if(this->m_affectation != nullptr){
        if(this->m_incrementation != nullptr){
            for(m_affectation->executer();m_expression->executer();m_incrementation->executer()){
                m_sequence->executer();
            }
        }
        else{
            for(m_affectation->executer();m_expression->executer();){
                m_sequence->executer();
            }
        }
    }
    else{
        if(this->m_incrementation != nullptr){
            for(m_affectation->executer();m_expression->executer();m_incrementation->executer()){
                m_sequence->executer();
            }
        }
        else{
            for(m_affectation->executer();m_expression->executer();){
                m_sequence->executer();
            }
        }
    }
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudLire
////////////////////////////////////////////////////////////////////////////////

NoeudLire::NoeudLire(vector<Noeud *>m_variables) : 
m_variables(m_variables){
}

int NoeudLire::executer() {
    for(auto elem : m_variables){
        int value;
        cin >> value;
        ((SymboleValue *)elem)->setValeur(value);
    }
}


    
////////////////////////////////////////////////////////////////////////////////
// NoeudEcrire
////////////////////////////////////////////////////////////////////////////////

NoeudEcrire::NoeudEcrire(vector<Noeud*> m_variable) :
m_variables(m_variable){
}

int NoeudEcrire::executer() {
    for(auto elem : m_variables){
        if ( (typeid(*elem)==typeid(SymboleValue) &&  *((SymboleValue*)elem)== "<CHAINE>" )){
            SymboleValue* s;
            s = dynamic_cast<SymboleValue*>(elem);
            string st = s->getChaine();
            st.erase(0,1);
            st.pop_back();
            cout<<st;
        }
        else{
            cout<<elem->executer();
        }
        cout<<endl;
    }
}


