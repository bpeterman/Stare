#include "serverv2.h"
#include "PHPsocket.h"

void serverv2::initEngine(CMDparser* cmdin)  {

	//socketVersion = MAKEWORD(2, 1);
	//WSAStartup(socketVersion, &windowsSocketData);
	//ip_address_size = sizeof(ip_address);

	//ip_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	//ip_address.sin_family = AF_INET;
	//ip_address.sin_port = htons(444);
        descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        info.sin_family = AF_INET;
	info.sin_addr.s_addr = INADDR_ANY;
	info.sin_port = htons(3456);
	memset(&info.sin_zero,0, sizeof(info.sin_zero));
	cmd=cmdin;
	

}

void serverv2::startServer() {

	//server_socket_listen = socket(AF_INET, SOCK_STREAM, NULL);

	//bind(server_socket_listen, (SOCKADDR*)&ip_address, ip_address_size);
	//listen(server_socket_listen, SOMAXCONN);
  
        if((bind(descriptor,(sockaddr *)&info, (socklen_t)sizeof(info))) >= 0){
	  listen(descriptor,5);
	  std::thread *tb = new std::thread(&serverv2::listening,this);
	  //listening();
	}

}


//----------------------------------------------------------------------------------------------------
// this method receives data from the socket. You have to tell it where the message ends
// otherwise it will print out garbage because of the unused buffer size initialized at the beginning.
//----------------------------------------------------------------------------------------------------

void serverv2::recvData(int client_socket) {

	   int recvDataSize = recv(client_socket, buffer, sizeof(buffer) - 1, NULL);
		// will implement error checking later. 
		if (recvDataSize >= 0) buffer[recvDataSize] = '\0';
		std::string recvDataString = buffer;
///     Get data from the algorithm/database in here. the two lines below are for testing purpose.
		PHPsocket php2(cmd);//cmd);
		std::cout << recvDataString << std::endl; 
		std::string result = php2.jsonDecoder(recvDataString);
		std::cout << result << std::endl;
		//std::cout << recvDataString << std::endl;
		//std::string dataToSend = "Hello client, I have received your data ";
		dataInChars = result.c_str();
		int dataSendSize = result.length();
		int sent_ok = send(client_socket, dataInChars, dataSendSize, NULL);
		
	}

void serverv2::listening(){
	
	while (true) {
	std::cout << "Waiting for client to connect " << std::endl;
	sockaddr_in sock_in;
	socklen_t tam = sizeof(sock_in);
	int client_socket = accept(descriptor, (struct sockaddr*)&sock_in, &tam);
	if (client_socket == -1)
	{
	  std::cout << "error making connection " << errno;
	}
	else
	{
	  std::cout << "A connection is found " << std::endl;
	  std::thread *tb = new std::thread(&serverv2::recvData, this, client_socket);
	    
	}
     }
}

