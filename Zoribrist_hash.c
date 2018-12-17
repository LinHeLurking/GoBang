//
// Created by Nine_yota on 2018-10-18.
//

#include "Zoribrist_hash.h"

unsigned long long hash_key[2][BOARD_SIZE][BOARD_SIZE] = {
        {
                {5299162016860989380UL,  5235120451384786018UL, 3493721262528985075UL,  18005677585128966595UL, 5852158464581675736UL,  4008844096682147058UL, 14423472971700828478UL, 6814765862548883971UL, 17613945114453206488UL, 16967694223713751487UL, 12188245011912688380UL, 16317129329630490314UL, 16973938642396026634UL, 17717140384438004840UL, 8252581252515900195UL},
                {4199756996475752604UL, 17028492798966212387UL, 1410474200642619518UL,  10409249947148202614UL, 1648844188269035835UL,  6582365980816920831UL, 9299421179451481916UL, 3753324942907125930UL,  17789030289862718571UL, 5867373269888495425UL, 3405543416952897767UL,  10027993795044601799UL, 10128006874606316155UL, 9732827178764973709UL,  6156215375719716595UL},
                {12919122721015759112UL, 3830716629798339509UL, 16681278826115738557UL, 14202032325748783491UL, 4460167769906497157UL, 7863929554482734468UL, 15042346957173003912UL, 2254689232870945753UL,  8463360314259312292UL,  13700561784339656198UL, 4634455935535180715UL,  17051393429554445719UL, 5680517557550738223UL,  12540208041661304164UL, 6980129662294642470UL},
                {6799354850504624621UL,  5114130323566505477UL,  5621921005513777117UL, 269613840063832570UL,  1887751031039368586UL, 18365344862723788510UL, 13182642633612900155UL, 333176556154199086UL,  8814648566079415208UL, 5818201487105629390UL,  3134005586975666615UL, 1429100121459142593UL, 9801675904263792733UL, 10210253351833975805UL, 5329705889201243256UL},
                {557981516005114069UL,  14974366444221083101UL, 303348788215388000UL,   18377269133014885461UL, 6514118334016608298UL,  5193175495903892438UL, 14318759343648996136UL, 6615423568354251974UL,  750239314468942188UL,   4616869435020695741UL, 9007753862803444194UL, 16172252204530810127UL, 12702918759669125481UL, 247381940995773478UL,  15242979079518294408UL},
                {10462177011121419174UL, 8348239583258610251UL, 6499002623595202267UL, 675223027099872789UL, 18108550434253323658UL, 17016162982871957469UL, 2149628354113835431UL,  15155878064376508325UL, 897260182325630274UL,  3686878576381630564UL,  6899888320916745192UL,  8133850460230648783UL, 11607063282733673106UL, 16624224993577152212UL, 9071591104751123145UL},
                {200898288526425261UL, 18120225894774743887UL, 19780135536040847UL,   5926420045251440115UL, 4910557796044540898UL, 9973518088960769354UL,  2200674518364128010UL,  10977121092229700955UL, 15497190460702432374UL, 7884104045639251036UL, 12229716394562173743UL, 11201116295316675124UL, 3166507672393184101UL,  17310973649007449433UL, 18360442937975963450UL},
                {2910863705416340746UL,  5869736614635260203UL,  9624758286795144931UL,  11541657407209352204UL, 17359316444465969870UL, 7230035275297154656UL,  9643548862593991UL,    14772916062389312067UL, 245428507518484727UL,  14730433420524207780UL, 12520365025472640595UL, 5355344812644339079UL,  153999943319892012UL,  11451541981860205053UL, 12672167044259516743UL},
                {2558602765200334978UL,  5939363815748143197UL,  8272709201808890171UL,  10464523255518800437UL, 11787841605387658997UL, 6341206803730414358UL, 18122342419962772488UL, 16229641902200098037UL, 16558270139155129042UL, 14752744278749312155UL, 18356944085480838015UL, 4932116187606859217UL,  17615140442713289954UL, 3094593723894371878UL,  3200032263023859290UL},
                {11922155694735579994UL, 215341389563134393UL,  3849776110582461800UL, 14224281177017250381UL, 6750523368820098151UL, 7428993458383209305UL, 10610359566560865700UL, 6999745298317709670UL,  10594359571193367493UL, 16762718098399298994UL, 17412130662955392303UL, 6594411726192298306UL,  8691255784501136932UL,  7263044022051301000UL,  11935411956700100113UL},
                {11790269449774506299UL, 10630613849053552490UL, 865653805274941356UL,  3653273889929378030UL,  17506896770465871409UL, 1985853191201101142UL,  6514608784113470938UL, 18048866851522762464UL, 10415149747834854246UL, 207684444156624758UL,   10349636226844416137UL, 7412722442647271259UL,  16010021630934838416UL, 8962541370231016666UL,  13799418904881000192UL},
                {5188988880327107757UL, 16431479070152145866UL, 7500006350591762725UL, 6826795150537598781UL, 5466508126916222618UL,  11605779385984221926UL, 10970318277461939558UL, 16875237053009139766UL, 16089764170206955360UL, 9809904111262380410UL,  7021172518622591226UL, 8174440797795583413UL,  9374213110029012420UL, 8232844120418579773UL,  11943559428593626088UL},
                {11617740016267356229UL, 5051628933063945334UL, 9421185011468801897UL,  1116714703003360642UL,  3726737230922925939UL,  477607210075068281UL,  3255088473002024999UL,  16341482321086295681UL, 11963894829694533764UL, 17724895939785368403UL, 12654497785388267722UL, 1278653772111777168UL, 10559141745317009465UL, 5608191006777643114UL, 617575518434916735UL},
                {6061428512428942502UL,  3779977646713817653UL, 5576464712245002879UL, 8856357178718394466UL, 18259654711913643891UL, 5735996409616540465UL,  5480430172522454256UL, 17546595782443456053UL, 5815880176122227953UL,  6414491171020364671UL,  8104946718542517472UL,  12962519282294211896UL, 8391583082278694033UL, 13610881170385449928UL, 7211869459053086680UL},
                {13670140623591839331UL, 14781054591496644767UL, 15077297823102436189UL, 3189378743411458772UL,  15560562339319456697UL, 2643293079832710396UL, 8110365401777832037UL,  4366833255481032623UL,  13652779349889266958UL, 17823523522647379458UL, 3307291821089649733UL, 10303383919380781699UL, 1745157423913957849UL,  8126923838816178692UL,  9556439772091873556UL}
        },
        {
                {10265301110374506860UL, 4949638757874223885UL, 17521238474031237098UL, 6719918244646533724UL,  13487404977160562812UL, 1290394698299663982UL, 755681002125600129UL,   5872640359626094005UL, 3720313650329495666UL,  14075422911994873697UL, 5428491783280721162UL,  12020504110117709221UL, 7910574969921098014UL,  15936039249078383765UL, 14944819856564503976UL},
                {9937942797582895068UL, 9403079469071026121UL,  10107403620123013945UL, 14718689751732124786UL, 11691821539847770989UL, 6202978864026174111UL, 9461626822666411423UL, 18151537731802609855UL, 1311853339629094610UL,  4469042288791758515UL, 11601217959527420790UL, 12553999760236786767UL, 8955841439238395407UL,  14635966089061299904UL, 16607148555433809437UL},
                {323458488821801857UL,   8916624704210283247UL, 13314061544355724074UL, 2553116821296128709UL,  6994192124476501544UL, 478266812116884210UL,  14993374300136359036UL, 12663998754447698707UL, 12950414515917740648UL, 3110404758756877291UL,  13363210393687878895UL, 16194999824911092917UL, 16200089370725915393UL, 13529491625460817173UL, 517116933811488713UL},
                {14356262786755047991UL, 18023236254581283502UL, 1621134609405669332UL, 3346189185679234214UL, 8495604835660965289UL, 2437737395488950770UL,  7248882932724577691UL,  1155025200913949268UL, 8505868204465504922UL, 15502421707433796916UL, 7058877797840433484UL, 1753284242908168284UL, 6278310353107515647UL, 10679054334631591018UL, 15826148661237830524UL},
                {5156875253854944974UL, 1822817194835238217UL,  18408126295943203305UL, 3466835899691369707UL,  10957525435011894729UL, 3303405552649711816UL, 8067347132050097543UL,  10344400345805868531UL, 12694211465006678877UL, 2312613260966520047UL, 4368519253409611802UL, 9696769889450838106UL,  18049448139061700630UL, 2233990836378092078UL, 1971852772968617973UL},
                {16726814752829520151UL, 7361807551216761843UL, 3089697903905153578UL, 790351079907402916UL, 4980583973405550806UL,  1028401383531443207UL,  17363473356284851500UL, 3874294444171449079UL,  3852938212017274394UL, 17820336016339004674UL, 12287075756402989734UL, 8504901532081838583UL, 7713230806298219461UL,  2060966541591913647UL,  9493948831390400010UL},
                {508051513421573602UL, 8822824568893009441UL,  5490638035549719030UL, 9089763495924169516UL, 5298441455947754121UL, 13355705575217694721UL, 18327598847044195650UL, 16457136888589498761UL, 3789166907099096392UL,  9332157124562403717UL, 5101736231480741128UL,  11617118827722863865UL, 12616540561838877071UL, 16579770581362651532UL, 11737737467317943427UL},
                {14498610289915395245UL, 14527412436676235668UL, 15033902631094355488UL, 17126191596519448596UL, 15567197258583385683UL, 14581772194123213983UL, 4256183779532933003UL, 8516827492527097007UL,  5843433875936121765UL, 11604368934629515852UL, 8300316884001664816UL,  12539452106902991909UL, 4704616014630695950UL, 2538348416137363815UL,  15587900535134731282UL},
                {11945592716583496792UL, 17099463035005756728UL, 15303351645253653360UL, 14400847557948097910UL, 8473924488810851802UL,  2044334181722162030UL, 413843180318392522UL,   663661985111602272UL,   12294382786892875019UL, 15660809300227518640UL, 17947347784966327579UL, 16201902957369794441UL, 13026708761978929143UL, 12957153419363349448UL, 15707103988022844255UL},
                {17782954658491327760UL, 4916859194470104924UL, 9712307215700274086UL, 14697923381068911264UL, 3549052659522637012UL, 9975693844818467374UL, 8875815909941944689UL,  11228277262554534031UL, 1422243992277895437UL,  15227128305965940454UL, 5779262302665532450UL,  17961189776804359431UL, 12929816413431136037UL, 13378750354489901898UL, 1844988310122781043UL},
                {11206263652160864593UL, 15690810661753702736UL, 1900646581376195357UL, 17397319541241053949UL, 8257455712452150126UL,  15842566566082872089UL, 1416947559839051794UL, 6364289515595009294UL,  3259452672497258983UL,  11924316578352462868UL, 3584073532529054033UL,  14769811974899496881UL, 16411723646587580752UL, 10687218151143813266UL, 15896184794941775365UL},
                {4007861535548380694UL, 2320764497993567793UL,  4543408432650805075UL, 2715018067095737057UL, 12090085791752779415UL, 3097386647679234594UL,  18122498470934360227UL, 7484493418653339627UL,  4411865467848573119UL,  11823064951403939018UL, 807128328410231007UL,  11569976311036159979UL, 8652095112428970510UL, 12832476182927148851UL, 836309420278085349UL},
                {4287546911077379016UL,  7600401170641084318UL, 13635593986906403928UL, 17802996551373286777UL, 10749107283405315942UL, 1541235156195686975UL, 13722913529948556536UL, 13789266356338172172UL, 6390867865089683751UL,  5725658147471988168UL,  14444413772322228018UL, 194904206784457850UL,  8156114170607223407UL,  349177074997516595UL,  10607474835796788439UL},
                {16309878695891294652UL, 5356839538415820619UL, 779954992759554004UL,  290335561616406420UL,  9836882945696023408UL,  14513673883336174096UL, 2239922071694447416UL, 3304412366481975277UL,  10462438024777959371UL, 12360284814358148003UL, 10330389197188791183UL, 16768704954855916755UL, 8960247604264611899UL, 8252695455720296738UL,  10908789953462650231UL},
                {13521926797905718472UL, 4141584215174367196UL,  10121631617051798888UL, 17052729127511737435UL, 11809988317209073280UL, 5470572097541027907UL, 13997325712659708548UL, 12568638517209944015UL, 234941429329931654UL,   10021196732013514760UL, 6642991032593117116UL, 17801130956244065473UL, 14036279267970785622UL, 17172950453486331668UL, 12828978785854559681UL}
        }
};


long long cache_total_grade[2][CACHE_SIZE];
long long cache_col_grade[2][CACHE_SIZE];
long long cache_row_grade[2][CACHE_SIZE];
long long cache_oblique_sum_grade[2][CACHE_SIZE];
long long cache_oblique_delta_grade[2][CACHE_SIZE];
int cache_record_step[CACHE_SIZE];
unsigned long long real_hash[CACHE_SIZE];
unsigned long long hash;

void hash_init() {
    SET0(cache_total_grade);
    SET0(cache_row_grade);
    SET0(cache_col_grade);
    SET0(cache_oblique_sum_grade);
    SET0(cache_oblique_delta_grade);
    SET0(real_hash);
    memset(cache_record_step, -1, sizeof(cache_record_step));

    hash = 0;
}
