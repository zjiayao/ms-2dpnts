par(mfrow=c(2,2))
x_lim <- c(-10, 80)
y_lim <- c(-10, 70)

dat <- read.table('oup', header=F, row.names=NULL)
plot(dat,xlim=x_lim, ylim=y_lim)

modes <- read.table('log', header=F, row.names=NULL, sep="")
plot(modes,xlim=x_lim, ylim=y_lim)

plot(dat,xlim=x_lim, ylim=y_lim)
files <- list.files(pattern="cluster_*")
i<-1
for(file in files)
{
	#if(i==1) {plot(read.csv(file, header=T, row.names=NULL), xlim=x_lim, ylim=y_lim, col=i, pch=i)}
	points(read.csv(file, header=T, row.names=NULL), col=i, pch=2*i-1)
	i <- i + 1
}

i<-1
for(file in files)
{
	if(i==1) {plot(read.csv(file, header=T, row.names=NULL), xlim=x_lim, ylim=y_lim, col=i, pch=i)}
	else {points(read.csv(file, header=T, row.names=NULL), col=i, pch=2*i-1)}
	i <- i + 1
}


