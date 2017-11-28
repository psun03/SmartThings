<?php
    $stWeek = 1;
    $dtTemp = getdate();
    $curDate =  strtotime($dtTemp["weekDay"] . " " . $dtTemp["year"] . "-" . $dtTemp["mon"] . "-" . $dtTemp["mday"]);
    //echo $curDate;
    $curWeek = idate('W', $curDate);
    $numWeeks = $curWeek - $stWeek;
    //echo '<br/>' . $curWeek;
    //echo '<br/>' . $numWeeks;
    if($numWeeks < 0) {
        $numWeeks * -1;
    }
    $answer = array('status'=>401, 'answer'=>'');
    if($numWeeks%2 == 1) {
        $answer['answer'] = utf8_encode('TR');
        $answer['status'] = 200;
    }
    else {
        $answer['answer'] = utf8_encode('T');
        $answer['status'] = 200;
    }
   //print_r($answer);
   header('Content-type: application/json');
   //echo json_encode($answer, JSON_FORCE_OBJECT);
   echo json_encode($answer);
?>