#ifndef ESBROOT_DATA_SUPERFGD_G4DIGIT_RECORD_H
#define ESBROOT_DATA_SUPERFGD_G4DIGIT_RECORD_H 1

#include <TObject.h>
#include <TClonesArray.h>
#include "EsbData/EsbSuperFGD/EsbDetector/EsbDigitReadout.h"

#include <iostream>

using namespace std;

namespace esbroot {

namespace data {

namespace superfgd {

namespace detector {

class G4DigitRecord : public TObject
{
public:
    /** Default constructor **/
    G4DigitRecord();

    /** Destructor **/
    ~G4DigitRecord();

    /** Add readout to the container
     *@param step - readout step
     **/
    void AddDigitReadout(const DigitReadout& step);

    /** Get readout
     *@param index - the index of readout in the container
     **/
    DigitReadout* GetDigitReadout(Int_t index) const;

    /** Get number of readouts in the container **/
    unsigned int NumDigitReadouts() const;

    /** Overriden Copy method from TObject**/
    void Copy(TObject& object) const;
    /** Overriden Clear method from TObject**/
    void Clear(Option_t* option = "");
    /** Overriden Print method from TObject**/
    void Print(Option_t* option = "") const;

    // Utility methods

    /** Initialize the readout container **/
    void Init();

    /** Reset the readout container **/
    void Reset();

    /** Print the readout container to stream **/
    void PrintToStream(ostream& stream) const;

private:
    TClonesArray* freadouts;

    ClassDef(G4DigitRecord, 2);
};

}   // detector
}   // superfgd
}   // data
}   // esbroot

#endif