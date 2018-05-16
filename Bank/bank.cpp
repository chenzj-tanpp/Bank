
#include <iostream>
#include<fstream>
#include <string>
using namespace std;



const char* Available_file = "Available.dat";//������Դ����, Available, ��������m,Available[j]Ϊ��ԴRj�Ŀ������� ��
const char* Max_file = "Max.dat";//����������Max, n��m��,Max[i][j]��ʾ����Pi����ԴRj�����������
const char* Allocation_file = "Allocation.dat";//�������Allocation, n��m��,Allocation[i][j]��ʾ����Pi�ѷֵ���ԴRj����
const char* Need_file = "Need.dat";//�������Need, n��m��,Need[i][j]��ʾ����Pi����ԴRj����������
const char* Request_file = "Request.dat";//��Դ������������Request[i], ��������m,Request[i][j]��ʾ����Pi����ԴRj����������
const char* Initi_file = "Initi.dat";//��ʼ����:��������,��Դ������

class Bank {
public:
	Bank();
	~Bank();
	void initi();
	void readFile(int pocess_sum, int resource_sum, int matrix[10][10], const char* file);
	void readFile(int resource_sum, int matrix[10], const char* file);
	void readIniti_file(const char* file);
	void setPocess_sum(int sum);
	void setResource_sum(int sum);
	void setRequest_T0(int sum);
	
	int getPocess_sum();
	int getSafe_queue(int m);
	int checkCurrentPocess(int currentPocess);
	int checkAll(int _NextLocation);

private:
	int available[10];//������Դ����
	int max[10][10];//����������
	int allocation[10][10];//�������
	int need[10][10];//�������
	int request[10][10];//��Դ������������
	int queue[10];//ִ�ж���
	int is_pocess_finished[10];//�����Ƿ�����
	int is_recur_finished[10];//��n��ݹ��Ƿ��ҵ���ȫ����

	int request_T0;//T0ʱ���ȷ����˭
	int resourceBuffer[10];//��ǰҪ�������Դ������,����ֻ����T0ʱ�̵�����
	int pocess_sum;//��������
	int resource_sum;//��Դ���� 
	int safeQueue[10];//��ȫ����
};

int Bank::getPocess_sum() {
	return pocess_sum;
}

int Bank::getSafe_queue(int m) {
	return safeQueue[m];
}

void Bank::readIniti_file(const char* file) {//��������,��Դ���� ,T0ʱ�̵�����
	ifstream input_file;
	input_file.open(file, ios_base::in);
	if (input_file.is_open()) {
		input_file >> pocess_sum >> resource_sum>>request_T0;
		if (request_T0 > -1) {
			for (int i = 0; i < resource_sum; i++) {
				input_file >> resourceBuffer[i];
			}
		}
		
	}
}

void Bank::readFile(int pocess_sum, int resource_sum, int matrix[10][10], const char* file) {
	ifstream input_file;
	input_file.open(file, ios_base::in);
	if (input_file.is_open()) {
		for (int i = 0; i < pocess_sum; i++) {
			for (int j = 0; j < resource_sum; j++) {
				input_file >> matrix[i][j];
			}
		}

	}
	input_file.close();
}

void Bank::readFile(int resource_sum, int matrix[10], const char* file) {
	ifstream input_file;
	input_file.open(file, ios_base::in);
	if (input_file.is_open()) {
		for (int i = 0; i < resource_sum; i++) {
			input_file >> matrix[i];
		}

	}
	input_file.close();
}

Bank::Bank(){}
Bank::~Bank(){}

void Bank::initi() {
	pocess_sum = 5;
	resource_sum = 3;
	request_T0 = -1;//T0ʱ��û������Ҫ��

	readIniti_file(Initi_file);
	readFile(resource_sum, available, Available_file);
	readFile(pocess_sum, resource_sum, max, Max_file);
	readFile(pocess_sum, resource_sum, allocation, Allocation_file);
	readFile(pocess_sum, resource_sum, need, Need_file);
	readFile(pocess_sum, resource_sum, request, Request_file);
	for (int i = 0; i < 10; i++) {
		is_pocess_finished[i] = 0;
		is_recur_finished[i] = 0;
		safeQueue[i] = -1;
	}

}

//0:���������ڿ�����,1:����������������,2:����������������
int Bank::checkCurrentPocess(int currentPocess) {
	for (int i = 0; i < resource_sum; i++) {
		if (request[currentPocess][i] > need[currentPocess][i]) return 2;//���п���request=need
		else {
			if (request[currentPocess][i] > available[i]) return 0;
			else if (request[currentPocess][i] <= available[i]) return 1;
		}
	}
}

int Bank::checkAll(int recursive = 0) {//recursive:�ݹ����
	int available_temp[10];//ʹ��temp�Ա�����Ҫʱ�ָ�����
	int allocation_temp[10][10];
	int need_temp[10][10];
	int request_temp[10][10];

	//��ʼ����-------------------------------
	if (request_T0 > -1) {
		int giveResBack = 0;//����ý��̱�����,��Ҫ�ͷ���Դ,���û�оͲ���Ҫ
		for (int i = 0; i < resource_sum; i++) {
			//if (resourceBuffer[i] > available[i]) 
			if (resourceBuffer[i] > need[request_T0][i]) {
				cout << "��ʼ����������������" << endl;
				return 2;
			}
			else {
				if (resourceBuffer[i] > available[i]) return 0;
				else if (resourceBuffer[i] <= available[i]) {
					// available_temp[i] = available[i] - resourceBuffer[i];
					// allocation_temp[request_T0][i] = allocation[request_T0][i] + resourceBuffer[i];
					// need_temp[request_T0][i] = need[request_T0][i] - resourceBuffer[i];

					available[i] = available[i] - resourceBuffer[i];
					allocation[request_T0][i] = allocation[request_T0][i] + resourceBuffer[i];
					need[request_T0][i] = need[request_T0][i] - resourceBuffer[i];
					request[request_T0][i] = request[request_T0][i] - resourceBuffer[i];
					if (need[request_T0][i] == 0) {
						giveResBack = giveResBack + 1;
					}
				}
			}
			if (giveResBack== resource_sum) {
				//�ý��̱�����,Ҫ�ͷ���Դ
				for (int res2 = 0; res2 < resource_sum; res2++) {
					available[res2] = available[res2]+max[request_T0][res2];
				}
			}
		}
		request_T0 = -1;//֮��ĵݹ�û�г�ʼ����,��Ϊ��ʼ�����Ѿ�������
	}
	//--------------------------------------------
	//Ѱ�Ұ�ȫ�Ľ��̶���
	int flag = -1;
	for (int i = 0; i < pocess_sum; i++) {
		if (is_pocess_finished[i]) {}
		else {
			int checkResult = checkCurrentPocess(i);
			if (checkResult == 1) {
				for (int j = 0; j < resource_sum; j++) {
					//��ʱ��request=need,����T0ʱ������������Ǹ�����
					// available_temp[j] = available[i] - need[i][j];
					// allocation_temp[i][j] = allocation[i][j] + need[i][j];
					// need_temp[i][j] = need[i][j] - need[i][j];
					available_temp[j] = available[j];
					allocation_temp[i][j] = allocation[i][j];
					need_temp[i][j] = need[i][j];
					request_temp[i][j] = request[i][j];

					available[j] = available[j] + max[i][j];//���̱�������ͷ���Դ
					allocation[i][j] = allocation[i][j] - need[i][j];
					need[i][j] = need[i][j] - need[i][j];
					request[i][j] = request[i][j] - request[i][j];

					is_pocess_finished[i] = 1;
					is_recur_finished[recursive] = 1;
					if (recursive >= 1) {
						is_recur_finished[recursive - 1] = 1;
					}

				}
				//recursive=recursive+1;
				if (recursive<pocess_sum - 1) {
					checkAll(recursive + 1);
					if (is_recur_finished[recursive] == 1) {
						safeQueue[recursive] = i;//��ȫ���е�recursive��λ���ǽ���i
						return 1;
					}
					if (is_recur_finished[recursive] == 0) {//�Ҳ�����һ����ȫ����
														   //
						is_pocess_finished[i] = 0;//�ý��̵����в���������
						for (int res2 = 0; res2 < resource_sum; res2++) {
							available[res2] = available_temp[res2];
							allocation[i][res2] = allocation_temp[i][res2];
							need[i][res2] = need_temp[i][res2];
							request[i][res2] = request_temp[i][res2];
						}
						
					}
				}
				else if (recursive == pocess_sum - 1) {
					safeQueue[recursive] = i;
					return 1;
					//�ݹ����,���ҵ���ȫ����
				}
				else if(recursive>pocess_sum - 1) {
					cout << "�ݹ����" << endl;
					return 3;//�ݹ����
				}


			}
			else {
				if (recursive >= 1) is_recur_finished[recursive - 1] = 0;
			}
		}

	}
	return 4;//û���ҵ���ȫ����

}

int main() {
	Bank bank_test;
	bank_test.initi();
	int result = bank_test.checkAll(0);
	int pocess_sum = bank_test.getPocess_sum();
	if (result == 1) {
		cout << "��ȫ����:" << endl;
		for (int i = 0; i < pocess_sum; i++) {
			cout << "  ����P" << bank_test.getSafe_queue(i) << "   ";
		}
	}
	else {
		cout << "�ް�ȫ����" << endl;
	}
	int end;
	cin >> end;
	return 0;
}

