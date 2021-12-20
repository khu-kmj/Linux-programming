#define	MY_ID		5		
#define	SHM_KEY		(0x9000 + MY_ID)
#define	SHM_SIZE	1024
#define	SHM_MODE	(SHM_R | SHM_W | IPC_CREAT)
#define SEM_KEY		(0x5000 + MY_ID)
