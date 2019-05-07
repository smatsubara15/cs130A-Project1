#include "HashSet.h"
#include <iostream>

HashSet::HashSet(){
  this->nitems=0;
  this->nslots=1000;
  this->intfn=new SquareRootHash(1,this->nslots);
  this->strfn=new JenkinsHash;
  this->strfn2=new JenkinsHash;
  this->slots=new std::string* [nslots];
  for(int i=0;i<nslots;i++)
    slots[i]=NULL;
}

HashSet::~HashSet(){
  delete intfn;
  delete strfn;
  delete strfn2;
  for(int i=0;i<nslots;i++)
      delete slots[i];
  delete [] slots;
}

void HashSet::insert(const std::string& value){
  uint64_t temp=strfn->hash(value);
  int index=intfn->hash(temp);
  double loadFactor=static_cast<double>(nitems)/static_cast<double>(nslots);
  if (loadFactor>0.5)
    rehash();
  while(slots[index]!=NULL){
    index++;
    index=index%nslots;
  }
  if(slots[index]==NULL){
    nitems++;
    slots[index]=new std::string(value);
  }
}

void HashSet::rehash(){
  std::string tempArray[nitems];
  int index=0;
  for(int i=0;i<nslots;i++){
    if(slots[i]!=NULL){
      tempArray[index]=*slots[i];
      delete slots[i];
      index++;
    }
  }
  delete [] slots;
  this->nslots=(this->nslots)*2;
  this->slots=new std::string* [nslots];
  for(int i=0;i<nslots;i++)
    slots[i]=NULL;
  this->intfn=new SquareRootHash(1,nslots);
  for(int i=0;i<index;i++){
    insert(tempArray[i]);
  }
}


bool HashSet::lookup(const std::string& value) const{
    uint64_t temp=strfn->hash(value);
    uint64_t index=intfn->hash(temp);
    while(slots[index]!=NULL){
      if(*slots[index]==value)
	return true;
      else{
	index++;
	index=index%nslots;
      }
    }
    return false;
}

