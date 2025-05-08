#include "action.hh"
#include "RunAction.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::BuildForMaster() const
{
SetUserAction(new RunAction);
}
void MyActionInitialization::Build() const 
{
MyPrimaryGenerator *generator = new MyPrimaryGenerator();
SetUserAction(generator);
    
SetUserAction(new RunAction);
}
