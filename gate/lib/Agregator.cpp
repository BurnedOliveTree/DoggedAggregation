#include "Agregator.h"

Agregator::Agregator(int nSerwers){
    for(int i = 0; i < nSerwers; i++){
        docBuilder.emplace_back();
        timestamps.emplace_back();
        packetCounter.emplace_back();
        error.emplace_back();
    }
}