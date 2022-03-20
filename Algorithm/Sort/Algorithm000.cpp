#include <iostream>

void PrintArray(int input[], int size)
{
    for(int i = 0; i < size; i++)
    {
        std::cout << input[i] << " ";
    }
    std::cout << std::endl;
}

void Swap(int& value1, int& value2)
{
    int temp = value1;
    value1 = value2;
    value2 = temp;
}



// Sequential Sort (순차 정렬)
// Time Complexity : O(n^2)
// Space Comlexity : O(n)
void SequentialSort(int input[], int size)
{
    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (input[i] > input[j])
            {
                Swap(input[i], input[j]);
            }
        }
    }
}
// size 만큼 순회해도 되지만 마지막은 비교 대상이 없어 최적화 할 여지가 있다. 
// size - 1 로 적을 수 있다. 엄청나게 큰 차이는 없다. 


// Selection Sort (작은걸 골라내서 정렬)
// Time Complexity : O(n^2)
// Space Complexity : O(n)
void SelectionSort(int input[], int size)
{
    for (int i = 0; i < size; i++)
    {
        int minIndex = i;
        for (int j = i; j < size; j++)
        {
            if (input[minIndex] > input[j])
            {
                minIndex = j;
            }

        }

        if (minIndex != i)
        {
            Swap(input[i], input[minIndex]);
        }
    }
}


// BubbleSort
// 인접한 두 개의 데이터를 비교해가면서 정렬 진행
//  ( 7 8 )  2 3 1 
// 큰걸 오른쪽으로 보냄
// 7 ( 8 2 )  3 1
// 8을 올리기 위해서 옮김
// 7 ( 2 8 ) 3 1
// 7 2 ( 8 3 ) 1
// 7 2 ( 3 8 ) 1
// 7 2 3 ( 8 1 )
// 7 2 3 ( 1 8 )
// 이렇게 한 스텝

// 그럼 여기서 8은 다 보내고 끝냈으니 마지막 제외하고 다시 실행
// ( 7 2 ) 3 1 '8'
// 2 ( 7 3 ) 1 '8'
// 2 3 ( 7 1 ) '8'
// 2 3 1 '7' '8'

// Time Complexity : O(n^2)
// Space Complexity : O(n)
void BubbleSort(int input[], int size)
{
    for (int phase = 0; phase < size - 1; phase++)
    {
        for (int left = 0; left < size - 1 - phase; left++)
        {
            if (input[left] > input[left + 1])
            {
                Swap(input[left], input[left + 1]);
            }
        }
    }

}


// Insertion Sort( 삽입 정렬 )
// 위치를 찾아 집어넣는다
// 결국 데이터를 옮기는데 N번 옮김 
// 정렬이 완료된 영역의 다음에 위치한 데이터가 다음 정렬 대상
// 삽입할 위치를 발견하고 데이터를 한 칸 뒤로 밀 수도 있지만, 데이터를 한 칸식 뒤로 밀면서 삽입 할 위치를 찾을수도 있다.

// Time Complexity : O(n^2)
// Space Complexity : O(n)
void InsertionSort(int input[], int size)
{
    for (int i = 1; i < size; i++)
    {
 //     int j = i;
        int target = input[i];

        int targetIndex{};

        //while(j-- >= 0 && target < input[i])
        //{
        //    input[j + 1] = input[j];
        //    input[j] = target;
        //}

        for (int j = i - 1; j >= 0; j--)
        {
            if (input[j] > target)
            {
                targetIndex = j;
                input[j + 1] = input[j];
                // 비교대상 한 칸 뒤로 밀기 
            }
            else {
                break; // 삽입 위치 찾았으니 나감
            }
        }

        input[targetIndex] = target;
        // 찾은 위치에 정렬 삽입
    }


}


// Divide & Conquer
// 분할 / 정복
// 어려운 문제가 주어지면 작은 문제로 만들자 

// 1단계 분할(Divide)  : 해결이 용이한 단계까지 문제를 분할
// 2단계 정복(Conquer) : 해결이 용이한 수준까지 분할된 문제 해결
// 3단계 결합(Combine) : 분할해서 해결한 결과를 결합하여 마무리

void Merge(int input[], int start, int half, int end, int temp[]);


// MergeSort ( 병합 정렬 )
void MergeSort(int input[], int start, int end, int temp[])
{
    //base
    if (start >= end)
    {
        return;
    }

    //recursive
    int half = (start + end) / 2;

    MergeSort(input, start, half, temp);
    MergeSort(input, half + 1, end, temp); // 반으로 나눔
    Merge(input, start, half, end, temp);
}

int main()
{
    const int SIZE{ 9 };

    int array[SIZE]{ 8, 7, 2, 3, 1 , 5, 6, 4, 9 };
    int tempArray[SIZE]{};
    
//    SequentialSort(array, SIZE);
//    SelectionSort(array, SIZE);
//    BubbleSort(array, SIZE);
//    InsertionSort(array, SIZE);

// 위에 것들은 성능상 큰 차이가 없다. 

    MergeSort(array, 0, SIZE - 1, tempArray);


    PrintArray(array, SIZE);
    
    int a = 0;
}

void Merge(int input[], int start, int half, int end, int temp[])
{
    int i = start;
    int j = half + 1;
    int tempIndex{};

    //두 블럭 비교 작은거 앞에
    while (i <= half && j <= end)
    {
        if (input[i] < input[j])
        {
            temp[tempIndex++] = input[i++];
        }
        else
        {
            temp[tempIndex++] = input[j++];
        }

    }

    // 남아있는게 있을때 왼쪽거 안에 집어 넣고 오른쪽거 뒤에 붙이기
    // 남은 임시 배열 저장 
    while (i <= half)
    {
        temp[tempIndex++] = input[i++];
    }
    while (j <= end)
    {
        temp[tempIndex++] = input[j++];
    }

    // 임시로 저장한걸 원본 배열에 다시 붙여넣기 
    tempIndex = 0;
    for (int i = start; i <= end; i++)
    {
        input[i] = temp[tempIndex++];
    }
}


// 수정 확인