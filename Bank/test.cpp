
#include "bank.cpp"

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
	return 0;
}