#ifdef CHANGED
#include "userthread.h"

struct thread_args{
  int f;
  int arg;
};

int
do_ThreadCreate(int f, int arg){
  //Création d'un nouveau thread qu'on lancera dans l'espace utilisateur
  Thread *thread = new Thread("newThread");
  thread -> Start((VoidFunctionPtr)f, &arg);
  //On sauvegarde l'argument et la fonction a faire passer au thread dans une structure a fin de les faire passer a StartUserThread
  struct thread_args *t_args = (thread_args*)malloc(sizeof(struct thread_args));
  t_args -> f = f;
  t_args -> arg = arg;
  //Faire passer le thread en utilisateur
  StartUserThread(t_args);
  //On renvoyer le status de thread crée
  return 0;
  //return thread -> status;
}

static void StartUserThread(void *schmurtz){
  int i;
  thread_args *thread = (thread_args*)schmurtz;

  //initialisation des registres
  for(i = 0 ; i < NumTotalRegs; i++)
    machine -> WriteRegister(i, 0);

  machine -> WriteRegister(4, thread -> arg);
  // Initial program counter -- must be location of "Start"
  machine -> WriteRegister(PCReg, thread -> f);

  // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
  machine->WriteRegister (NextPCReg, machine->ReadRegister(PCReg) + 4);
  
  // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!
  machine->WriteRegister (StackReg, currentThread -> space -> AllocateUserStack());
  
  
  machine -> Run();
  
  free(schmurtz);
}
#endif //CHANGED
