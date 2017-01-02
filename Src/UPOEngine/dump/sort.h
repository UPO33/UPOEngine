//bbl sort
// 	int n = ((int)array.Length()) - 1;
// 		for (int x = 0; x < n; x++)
// 		{
// 			for (int y = 0; y < n; y++)
// 			{
// 				if (arr[y] > arr[y + 1])
// 				{
// 					auto temp = arr[y + 1];
// 					arr[y + 1] = arr[y];
// 					arr[y] = temp;
// 				}
// 			}
//
// 		}
// 
// 	for (int i = 0; i < n; i++)
// 	{
// 		for (int j = 0; j < n - i; j++)
// 		{
// 			if (proc(array[j], array[j + 1])) /* For decreasing order use < */
// 			{
// 				auto swap = array[j];
// 				array[j] = array[j + 1];
// 				array[j + 1] = swap;
// 			}
// 		}
// 	}