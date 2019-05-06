#include "BloomFilter.h"
#include <algorithm>
#include <iostream>

BloomFilter::BloomFilter(int k, int m, std::string strfn, std::string intfn){
  this->k=k;
  this->m=m;
  this->bits= new uint64_t[m];
  for(int i=0;i<m;i++){
    bits[i]=0;
  }
  std::transform(strfn.begin(),strfn.end(),strfn.begin(), ::tolower);
  std::transform(intfn.begin(),intfn.end(),intfn.begin(), ::tolower);
  if(strfn=="jenkins")
    this->strfn=new JenkinsHash;
  else if(strfn=="pearson")
    this->strfn=new PearsonHash;
  else{
    std::cerr<<"Invalid string function"<<std::endl;
  }
  this->intfns=new IntegerHash*[k];
  if(intfn=="division"){
    //this->intfns=new DivisionHash*[k];
    for(int i=0;i<k;i++){
      intfns[i]=new DivisionHash(i,m);
    }
  }
  else if(intfn=="reciprocal"){
    //this->intfns=new ReciprocalHash*[k];
    for(int i=0;i<k;i++){
      intfns[i]=new ReciprocalHash(i,m);
    }
  }
  else if(intfn=="squareroot"){
    //this->intfns=new SquareRootHash*[k];
    for(int i=0;i<k;i++){
      intfns[i]=new SquareRootHash(i,m);
    }
  }
  else{
    std::cerr<<"Invlaed integer function"<<std::endl;
  }
}

BloomFilter::~BloomFilter(){
  delete strfn;
  for(int i=0;i<k;i++){
    delete intfns[i];
  }
  delete [] intfns;
  delete [] bits;
}

void BloomFilter::insert(const std::string& value){
  uint64_t temp=strfn->hash(value);
  for (int i=0;i<k;i++){
    uint64_t index=intfns[i]->hash(temp);
    bits[index]=1;
  }
}

bool BloomFilter::lookup(const std::string& value) const{
  uint64_t temp=strfn->hash(value);
  int counter=0;
  for(int i=0;i<k;i++){
    uint64_t index=intfns[i]->hash(temp);
    if(bits[index]==1){
      counter++;
    }
  }
  if(counter==k)
    return true;
  else
    return false;
}
    
