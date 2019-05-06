#include "HashSet.h"
#include <iostream>

HashSet::HashSet(){
  this->nitems=0;
  this->nslots=100;
  this->intfn=new SquareRootHash(1,nslots);
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
  for(int i=0;i<nslots;i++){
    if (slots[i]!=NULL)
      delete slots[i];
  }
  delete [] slots;
}

void HashSet::insert(const std::string& value){
  uint64_t temp=strfn->hash(value);
  int index=intfn->hash(temp);
  double loadFactor=static_cast<double>(nitems)/static_cast<double>(nslots);
  if (loadFactor>0.5)
    rehash();
  while(slots[index]!=NULL && *slots[index]!=value && index<nslots){
    index++;
    //    index=index%nslots;
  }
  if(slots[index]==NULL){
    nitems++;
    //    const std::string* tempString=value;
    slots[index]=new std::string(value);
  }
}

void HashSet::rehash(){
  std::string tempArray[nitems];
  int index=0;
  for(int i=0;i<nslots;i++){
    if(slots[i]!=NULL){
      tempArray[index]=*slots[i];
      index++;
    }
  }
  this->nslots=(this->nslots)*2;
  this->slots=new std::string* [nslots];
  this->intfn=new SquareRootHash(1,nslots);
  for(int i=0;i<nitems;i++){
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

//void HashSet::print(){
  //for (int i=0;i<nslots;i++){
    //    if(slots[i]!=NULL){
    //std::cout<<&slots[i]<<std::endl;
      //}
      // }
  //}
