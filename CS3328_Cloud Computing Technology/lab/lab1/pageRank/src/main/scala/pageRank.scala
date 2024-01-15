import org.apache.spark.graphx._
import org.apache.spark.sql.SparkSession

object PageRank {
  def main(args: Array[String]): Unit = {
    val spark = SparkSession.builder.appName(s"${this.getClass.getSimpleName}").getOrCreate()
    val sc = spark.sparkContext

    val inputPath = if (args.length > 0) args(0) else "data/graphx/Wiki-Vote.txt"
    val dampingFactor = if (args.length > 1) args(1).toDouble else 0.85
    val epoch = if (args.length > 2) args(2).toInt else 100

    val graph: Graph[Int, Int] = GraphLoader.edgeListFile(sc, inputPath)

    var ranks = graph.outerJoinVertices(graph.outDegrees) {
      (vertexId, _, degreeOption) => degreeOption.getOrElse(0)
    }.mapTriplets(e => 1.0 / e.srcAttr).mapVertices((_, _) => 1.0)

    for (_ <- 1 to epoch) {
      val contributions = ranks.aggregateMessages[Double](
        context => context.sendToDst(context.srcAttr * context.attr), _ + _)
      ranks = ranks.outerJoinVertices(contributions) {
        (id, rank, contribution) => 1.0 - dampingFactor + dampingFactor * contribution.getOrElse(0.0)
      }
      ranks.cache()
    }

    val topRanks = ranks.vertices.collect().sortBy(- _._2).take(20)
    println(topRanks.mkString("\n"))

    spark.stop()
  }
}
