#pragma once
#include "../3sBid/sBid.h"

class Server {
private:
	string IP = "0.0.0.0";
	int port = 18000;
	struct socket_info
	{
		struct sockaddr_in sktaddr;
		int fd;
	};
	int MAX_CONN = 100;
	socket_info soc[100];
public:
	int start() {
		socket_info listenskt, connskt;
		pthread_t tid;
		socklen_t clit_size;
		socket_info soc[MAX_CONN];   //�������ÿ�����ӵ���Ϣ
		int i = 0;

		memset(&listenskt, 0, sizeof(listenskt));
		memset(&connskt, 0, sizeof(connskt));

		if ((listenskt.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			cout << "socket create failed : " << strerror(errno) << endl;
			return 0;
		}

		listenskt.sktaddr.sin_family = AF_INET;
		listenskt.sktaddr.sin_port = htons(port);
		listenskt.sktaddr.sin_addr.s_addr = inet_addr(IP.c_str());

		if (bind(listenskt.fd, (sockaddr*)&listenskt.sktaddr, sizeof(listenskt.sktaddr)) == -1)
		{
			cout << "bind error : " << strerror(errno) << endl;
			return 0;
		}

		if (listen(listenskt.fd, 100) == -1)
		{
			cout << "listen error : " << strerror(errno) << endl;
			return 0;
		}

		cout << "Init Success ! " << endl;
		cout << "host ip : " << inet_ntoa(listenskt.sktaddr.sin_addr) << "  port : " << ntohs(listenskt.sktaddr.sin_port) << endl;

		cout << "Waiting for connections ... " << endl;

		while (1)
		{
			clit_size = sizeof(connskt.sktaddr);
			if ((connskt.fd = accept(listenskt.fd, (sockaddr*)&connskt.sktaddr, &clit_size)) == -1)
			{
				if (errno == EINTR)continue;  //���������һ�䣬��һ���ͻ��˶Ͽ�ʱ�������ж��쳣��Ҳ���������EINTR��ͨ����仰������accept����Ȼ�ͻ�ֱ���˳�
				cout << "accept error : " << strerror(errno) << endl;
				return 0;
			}
			memcpy(&soc[i], &connskt, sizeof(connskt)); //�������ӵ���Ϣ������soc������

			pthread_create(&tid, NULL, connfun, (void*)&soc[i++]);
			pthread_detach(tid);

			if (i >= MAX_CONN)  //������������࣬�Ͳ��ٽ�������������
			{
				cout << "Reach the max connections : " << strerror(errno) << endl;
				break;
			}
		}

		close(listenskt.fd);
		return 0;
	}

	static void* connfun(void* arg)  //�߳���ڣ�����ͻ��������˵�ͨ��
	{
		socket_info* connskt = (socket_info*)arg;  //����ǿ��ת��
		cout << inet_ntoa((*connskt).sktaddr.sin_addr) << ":" << ntohs((*connskt).sktaddr.sin_port) << " connected ...  " << endl;
		while (1)
		{
			char buf[1024];

			int readstate = read(connskt->fd, buf, sizeof(buf));

			if (readstate == -1)
			{
				cout << "read error : " << strerror(errno) << endl;
				break;
			}
			else if (readstate == 0)  //�ͻ����˳�
			{
				cout << inet_ntoa((*connskt).sktaddr.sin_addr) << ":" << ntohs((*connskt).sktaddr.sin_port) << " exit ... " << endl;
				break;
			}
			write(STDOUT_FILENO, buf, readstate);  //��ӡ�յ�������
			cout << "      (From " << inet_ntoa((*connskt).sktaddr.sin_addr) << ":" << ntohs((*connskt).sktaddr.sin_port) << ")" << endl;
			for (int i = 0; i < readstate; i++)buf[i] = toupper(buf[i]); //��ĸתΪ��д
			write(connskt->fd, buf, readstate);  //���ؿͻ���	
		}
		close(connskt->fd);  //�رյ�ǰ������ʹ�õ��ļ�������
		return NULL;
	}

};