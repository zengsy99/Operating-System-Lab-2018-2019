五个文件夹分别包含四个Task和1个Bonus。

其中Task4的优先级调度体现在：测式任务schedule中的第一个打印文字任务（即上板运行后第三行）初始化的时候优先级较低，因此前几轮不会打印，所以第三行打印出来的数字会比第四行小。（因为Task4能体现优先级，因此Task3没有再体现，若想在Task3中体现优先级调度，只需要将初始化的时候，改变某个PCB的优先级，并写到相应的ready_queue中即可）

Bonus的优先级调度体现在打印顺序上，即打印顺序为1->3->2.