在invest.aql中，需要提取的语句含义是：company + invest + amount + program （即公司在某个项目投资多少）
首先，amount是由number + unit 组成的，因此需要先提取出number，unit的view，将其通过pattern语句拼接成amount。
然后，提取出company，invest和program的view，再通过pattern将这四个view按照company + invest + amount + program的顺序拼接成view：CompanyForProgram，可以得到目标语句。
