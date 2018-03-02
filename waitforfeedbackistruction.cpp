#include "waitforfeedbackistruction.h"

WaitForFeedbackIstruction::WaitForFeedbackIstruction(QObject *parent)
	: QThread(parent)
{
	waitStatus = false;
}

WaitForFeedbackIstruction::~WaitForFeedbackIstruction()
{

}

void WaitForFeedbackIstruction::run()
{
	int i = 0;
	while (waitStatus){
		msleep(5);//����5����
		i++ ;
		if (i == 1000){
			emit sendAgain();
			break;
		}

	}
}
