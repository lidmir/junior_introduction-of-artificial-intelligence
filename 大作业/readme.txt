程序：
运行环境：MATLAB R2015b

通过比较马尔科夫链和BP神经网络两种方法分别对一年内的黄连价格进行预测，我发现对于多因素干扰下的价格变化，在仅仅通过价格预测价格而且可用数据比较丰富的情况下，BP神经网络较马尔科夫链预测的结果比较高效和准确。因为马尔科夫链仅仅通过状态转换状态的概率高低来预测下个月可能的价格趋势变化，而且由于实际的客观事物很难长期保持同一状态转移概率，马尔科夫链只能适应短期预测；而在很多因素影响下的黄连市场并不是只有价格这样的因素在影响其价格的变化，还有很多如同天气、疫情等因素在影响着价格变化。

huanglian.xlsx                  ——1988到2016年每个月的黄连价格

bp.m				——BP神经网络预测药材黄连每年12月份的价格的matlab程序
testData.txt 			——bp.m用到的测试数据输入
testOutput.txt			——bp.m用到的测试数据输出
trainData.txt			——bp.m用到的训练数据输入
trainOutput.txt			——bp.m用到的训练数据输出

Markov.m 			——马尔可夫链预测药材黄连一年的价格的matlab程序
markovData.txt			——Markov.m用到的统计数据

bp1.m 				——BP神经网络预测药材黄连一年的价格的matlab程序
testData1.txt 			——bp1.m用到的测试数据输入
testOutput1.txt			——bp1.m用到的测试数据输出
trainData1.txt			——bp1.m用到的训练数据输入
trainOutput1.txt		——bp1.m用到的训练数据输出

