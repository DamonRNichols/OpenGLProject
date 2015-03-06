#include <thread>
#include <mutex>
#include <vector>
using std::vector;

#include "glm_header.h"

void print(int i)
{
	static std::mutex myMutex;
	std::lock_guard<std::mutex> guard(myMutex);
	printf("Hello Thread %i\n");
	printf("I'm here...\n");
	printf("...not there.\n");
}


//void main()
//{
//	std::vector<std::thread> threads;
//	vec4 myVectors[5000];
//	int chunkLength = 5000 / 10;
//	for (int i = 0; i < 10; i++) {
//		threads.push_back(std::thread(
//			// defining a lambda that can access main scope with &
//			[&](int low, int high, int thread_num){
//			for (int j = low; j < high; j++)
//			{
//				myVectors[i] = normalize(myVectors[i]);
//				printf("Vector.x = %f", myVectors[i].x);
//				printf(", Vector.y = %f", myVectors[i].y);
//				printf(", Vector.z = %f\n", myVectors[i].z);
//			}
//			printf("thread number: %i\n", thread_num);
//		}, i * chunkLength, (i + 1) * chunkLength, i
//			));
//	}//
//	for (auto& thread : threads)
//		thread.join();
//
//	system("pause");
//}